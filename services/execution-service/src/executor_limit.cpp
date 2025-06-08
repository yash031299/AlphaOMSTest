#include "executor_limit.hpp"
#include <spdlog/spdlog.h>
#include <cmath>

LimitOrderExecutor::LimitOrderExecutor(const std::string& symbol)
    : symbol_(symbol), lastExecutedPrice_(0.0) {}

bool LimitOrderExecutor::execute(double price, double quantity, int side) {
    SPDLOG_INFO("[LIMIT] Executing {} order for {} → Qty: {:.2f} @ {:.2f}",
                (side == 1 ? "BUY" : "SELL"), symbol_, quantity, price);

    // Basic validation
    if (price <= 0 || quantity <= 0) {
        SPDLOG_WARN("[LIMIT] Invalid order params");
        return false;
    }

    // Simulate fill logic
    // You can later replace this with matching engine or order book callback
    double ltp = price;  // assume match at desired price
    lastExecutedPrice_ = ltp;

    SPDLOG_INFO("[LIMIT] Order filled at {:.2f} for {}", ltp, symbol_);
    return true;
}
