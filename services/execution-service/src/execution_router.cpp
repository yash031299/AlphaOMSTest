#include "execution_router.hpp"
#include "executor_limit.hpp"
#include "executor_market.hpp"
#include "executor_sl.hpp"
#include "redis_publisher.hpp"
#include "pnl_client.hpp"
#include "user_client.hpp"
#include <spdlog/spdlog.h>

static int total_orders = 0;
static int successful_executions = 0;
static int rejected_orders = 0;


int parseSide(const std::string& sideStr) {
    if (sideStr == "BUY" || sideStr == "buy") return 1;
    if (sideStr == "SELL" || sideStr == "sell") return -1;
    return 0;  // fallback for invalid
}

// Helper implementations
order::OrderResponse executeLimitOrder(const order::OrderRequest& req) {
    order::OrderResponse res;
    LimitOrderExecutor exec(req.symbol());
    bool ok = exec.execute(req.price(), req.quantity(), parseSide(req.side()));
    res.set_status(ok ? "ACCEPTED" : "REJECTED");
    res.set_message(ok ? "Limit order executed" : "Limit order failed");
    return res;
}

order::OrderResponse executeMarketOrder(const order::OrderRequest& req) {
    order::OrderResponse res;
    MarketOrderExecutor exec(req.symbol());
    bool ok = exec.execute(req.ltp(), req.quantity(), parseSide(req.side()));
    res.set_status(ok ? "ACCEPTED" : "REJECTED");
    res.set_message(ok ? "Market order executed" : "Market order failed");
    return res;
}

order::OrderResponse executeSLOrder(const order::OrderRequest& req) {
    order::OrderResponse res;
    SLOrderExecutor exec(req.symbol());
    bool ok = exec.execute(req.ltp(), req.stopprice(), req.quantity(), parseSide(req.side()));
    res.set_status(ok ? "ACCEPTED" : "REJECTED");
    res.set_message(ok ? "SL order triggered" : "SL not triggered");
    return res;
}


order::OrderResponse ExecutionRouter::route(const order::OrderRequest& req) {
    order::OrderResponse resp;
    ++total_orders;

    try {
        if (req.ordertype() == "LIMIT") {
            resp = executeLimitOrder(req);
        } else if (req.ordertype() == "MARKET") {
            resp = executeMarketOrder(req);
        } else if (req.ordertype() == "SL") {
            resp = executeSLOrder(req);
        } else {
            resp.set_status("REJECTED");
            resp.set_message("Unknown order type");
            return resp;
        }

        // Redis + PnL only if successful
        if (resp.status() == "ACCEPTED") {
            ++successful_executions;
            double exec_price = req.ordertype() == "LIMIT" ? req.price() :
                                req.ordertype() == "MARKET" ? req.ltp() :
                                req.stopprice();

            // Calculate fee
            double fee_bps = std::getenv("EXEC_FEE_BPS") ? std::stod(std::getenv("EXEC_FEE_BPS")) : 10.0;
            double fee = (exec_price * req.quantity()) * (fee_bps / 10000.0);
            double net_qty = req.quantity() - fee;

            // Trade ID generation
            std::string trade_id = "TRADE_" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count());

            RedisPublisher redis;
            redis.publishTrade(req, trade_id);

            std::string pnl_host = std::getenv("PNL_GRPC_ADDR") ? std::getenv("PNL_GRPC_ADDR") : "localhost:50056";
            PnLClient pnl(grpc::CreateChannel(pnl_host, grpc::InsecureChannelCredentials()));
            pnl.sendTradeUpdate(req.user_id(), req.symbol(), exec_price, net_qty, req.side());

            std::string user_host = std::getenv("USER_GRPC_ADDR") ? std::getenv("USER_GRPC_ADDR") : "localhost:50052";
            UserWalletClient wallet(grpc::CreateChannel(user_host, grpc::InsecureChannelCredentials()));
            wallet.updateWallet(req.user_id(), -fee, "execution_fee");
        } else {
            ++rejected_orders;

            std::string trade_id = "REJECT_" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count());

            try {
                RedisPublisher redis;
                redis.publishReject(req, resp.message());
                SPDLOG_WARN("🚫 Order rejected and published to STREAM:REJECT");
            } catch (const std::exception& e) {
                SPDLOG_ERROR("Failed to publish rejection to Redis: {}", e.what());
            }
        }


    } catch (const std::exception& ex) {
        resp.set_status("ERROR");
        resp.set_message(std::string("Exception: ") + ex.what());
    }

    return resp;
}

