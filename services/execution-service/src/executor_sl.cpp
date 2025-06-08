#include "executor_sl.hpp"
#include <spdlog/spdlog.h>

SLOrderExecutor::SLOrderExecutor(const std::string& symbol)
    : symbol_(symbol), lastTriggeredPrice_(0.0) {}

bool SLOrderExecutor::execute(double ltp, double stopPrice, double quantity, int side) {
    if (ltp <= 0 || stopPrice <= 0 || quantity <= 0) {
        SPDLOG_WARN("[SL] Invalid stop or quantity");
        return false;
    }

    bool trigger = false;

    if (side == 1 && ltp >= stopPrice) trigger = true;        // BUY SL triggers when price rises
    else if (side == -1 && ltp <= stopPrice) trigger = true;  // SELL SL triggers when price falls

    if (trigger) {
        lastTriggeredPrice_ = ltp;
        SPDLOG_INFO("[SL] Triggered {} SL @ {:.2f} for {} (Stop: {:.2f}, Qty: {:.2f})",
                    (side == 1 ? "BUY" : "SELL"), ltp, symbol_, stopPrice, quantity);
        return true;
    }

    SPDLOG_INFO("[SL] Not triggered: {} (LTP: {:.2f}, Stop: {:.2f})",
                symbol_, ltp, stopPrice);
    return false;
}
