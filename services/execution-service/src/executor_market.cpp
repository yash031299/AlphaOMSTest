#include "executor_market.hpp"
#include <spdlog/spdlog.h>

MarketOrderExecutor::MarketOrderExecutor(const std::string& symbol)
    : symbol_(symbol), lastExecutedPrice_(0.0) {}

bool MarketOrderExecutor::execute(double ltp, double quantity, int side) {
    if (ltp <= 0 || quantity <= 0) {
        SPDLOG_WARN("[MARKET] Invalid LTP or quantity.");
        return false;
    }

    SPDLOG_INFO("[MARKET] Executing {} market order for {} → Qty: {:.2f} @ LTP {:.2f}",
                (side == 1 ? "BUY" : "SELL"), symbol_, quantity, ltp);

    lastExecutedPrice_ = ltp;

    // In real engine: would check slippage, depth, etc.

    SPDLOG_INFO("[MARKET] Order filled at market price {:.2f} for {}", ltp, symbol_);
    return true;
}
