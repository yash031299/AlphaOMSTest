#include "execution_router.hpp"
#include "executor_limit.hpp"
#include "executor_market.hpp"
#include "executor_sl.hpp"
#include "redis_publisher.hpp"
#include "pnl_client.hpp"
#include "user_client.hpp"


order::OrderResponse ExecutionRouter::route(const order::OrderRequest& req) {
    order::OrderResponse resp;

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
            double exec_price = req.ordertype() == "LIMIT" ? req.price() :
                                req.ordertype() == "MARKET" ? req.ltp() :
                                req.stopprice();

            RedisPublisher redis;
            redis.publishTrade(req.symbol(), req.user_id(), exec_price, req.quantity(), req.side());

            PnLClient pnl(grpc::CreateChannel("pnl-service:50056", grpc::InsecureChannelCredentials()));
            pnl.sendTradeUpdate(req.user_id(), req.symbol(), exec_price, req.quantity(), req.side());
        }

    } catch (const std::exception& ex) {
        resp.set_status("ERROR");
        resp.set_message(std::string("Exception: ") + ex.what());
    }

    return resp;
}

// Helper implementations
order::OrderResponse executeLimitOrder(const order::OrderRequest& req) {
    order::OrderResponse res;
    execution::LimitOrderExecutor exec(req.symbol());
    bool ok = exec.execute(req.price(), req.quantity(), req.side());
    res.set_status(ok ? "ACCEPTED" : "REJECTED");
    res.set_message(ok ? "Limit order executed" : "Limit order failed");
    return res;
}

order::OrderResponse executeMarketOrder(const order::OrderRequest& req) {
    order::OrderResponse res;
    execution::MarketOrderExecutor exec(req.symbol());
    bool ok = exec.execute(req.ltp(), req.quantity(), req.side());
    res.set_status(ok ? "ACCEPTED" : "REJECTED");
    res.set_message(ok ? "Market order executed" : "Market order failed");
    return res;
}

order::OrderResponse executeSLOrder(const order::OrderRequest& req) {
    order::OrderResponse res;
    execution::SLOrderExecutor exec(req.symbol());
    bool ok = exec.execute(req.ltp(), req.stopprice(), req.quantity(), req.side());
    res.set_status(ok ? "ACCEPTED" : "REJECTED");
    res.set_message(ok ? "SL order triggered" : "SL not triggered");
    return res;
}
