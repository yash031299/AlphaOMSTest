#include "pnl_client.hpp"
#include <spdlog/spdlog.h>

PnLClient::PnLClient(std::shared_ptr<grpc::Channel> channel)
    : stub_(pnl::PnLService::NewStub(channel)) {}

void PnLClient::sendTradeUpdate(const std::string& user_id, const std::string& symbol,
                                double price, double quantity, const std::string& side) {
    pnl::TradeUpdate req;
    req.set_user_id(user_id);
    req.set_symbol(symbol);
    req.set_price(price);
    req.set_quantity(quantity);
    req.set_side(side);

    pnl::Empty ack;
    grpc::ClientContext ctx;

    auto status = stub_->SendTrade(&ctx, req, &ack);
    if (!status.ok()) {
        SPDLOG_ERROR("PnL gRPC failed: {}", status.error_message());
    } else {
        SPDLOG_INFO("PnL trade update sent for user {}", user_id);
    }
}
