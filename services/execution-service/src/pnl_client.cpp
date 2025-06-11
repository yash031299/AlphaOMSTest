#include "pnl_client.hpp"
#include <spdlog/spdlog.h>
#include <cstdlib>
#include <thread>
#include <chrono>
#include "pnl.pb.h"
#include "pnl.grpc.pb.h"

PnLClient::PnLClient(std::shared_ptr<grpc::Channel> channel)
    : stub_(pnl::PnLService::NewStub(channel)) {}

bool PnLClient::sendTradeUpdate(const std::string& user_id,
                                const std::string& symbol,
                                double price,
                                double qty,
                                const std::string& side) {
    pnl::TradeUpdate request;
    request.set_user_id(user_id);
    request.set_symbol(symbol);
    request.set_price(price);
    request.set_quantity(qty);
    request.set_side(side);

    grpc::Status status;
    for (int attempt = 1; attempt <= 3; ++attempt) {
        grpc::ClientContext context;
        pnl::Empty response;

        try {
            status = stub_->SendTrade(&context, request, &response);
        } catch (const std::exception& e) {
            SPDLOG_ERROR("❌ gRPC exception on attempt {}: {}", attempt, e.what());
        } catch (...) {
            SPDLOG_ERROR("❌ Unknown gRPC exception on attempt {}", attempt);
        }

        if (status.ok()) {
            SPDLOG_INFO("✅ PnL update succeeded on attempt {} for user: {}", attempt, user_id);
            return true;
        }

        SPDLOG_WARN("⚠️ PnL update failed (attempt {}): {}", attempt, status.error_message());
        std::this_thread::sleep_for(std::chrono::milliseconds(100 * (1 << (attempt - 1))));
    }

    SPDLOG_ERROR("❌ PnL update failed after all retries for user: {}", user_id);
    return false;
}