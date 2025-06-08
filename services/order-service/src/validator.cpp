#include "validator.hpp"
#include <spdlog/spdlog.h>

Validator::Validator(std::shared_ptr<SymbolClient> symbol_client,
                     std::shared_ptr<UserClient> user_client)
    : symbol_client(std::move(symbol_client)), user_client(std::move(user_client)) {}

bool Validator::validateOrder(const order::OrderRequest &order) {
    auto meta = symbol_client->getSymbolMeta(order.symbol());
    if (!meta) {
        SPDLOG_WARN("Validation failed: Symbol not found - {}", order.symbol());
        return false;
    }

    if (order.leverage() > meta->max_leverage) {
        SPDLOG_WARN("Validation failed: Leverage {} exceeds max {} for symbol {}",
                    order.leverage(), meta->max_leverage, order.symbol());
        return false;
    }

    auto user_meta = user_client->getUserMeta(order.user_id());
    if (!user_meta) {
        SPDLOG_WARN("Validation failed: user {} metadata not found", order.user_id());
        return false;
    }

    if (!user_meta->kyc_verified) {
        SPDLOG_WARN("Validation failed: user {} is not KYC verified", order.user_id());
        return false;
    }

    auto wallet = user_client->getWallet(order.user_id());
    if (!wallet) {
        SPDLOG_WARN("Validation failed: wallet not found for user {}", order.user_id());
        return false;
    }

    double cost_estimate = order.price() * order.quantity() / order.leverage();
    if (wallet->available_balance < cost_estimate) {
        SPDLOG_WARN("Validation failed: insufficient margin. Required: {}, Available: {}",
                    cost_estimate, wallet->available_balance);
        return false;
    }

    return true;
}
