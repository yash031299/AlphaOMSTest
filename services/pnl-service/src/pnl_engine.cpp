#include "pnl_engine.hpp"

PnLEngine::PnLEngine(const std::string& userId, double wallet)
    : userId_(userId), wallet_(wallet) {}

void PnLEngine::updatePosition(const std::string& symbol, double qty, double entryPrice) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto& pos = positions_[symbol];

    // Average-in logic
    double totalQty = pos.quantity + qty;
    if (totalQty == 0) {
        wallet_ += (qty * (pos.avgEntry - entryPrice)); // Realized PnL on full exit
        pos.quantity = 0;
        pos.avgEntry = 0;
        return;
    }

    pos.avgEntry = (pos.quantity * pos.avgEntry + qty * entryPrice) / totalQty;
    pos.quantity = totalQty;
}

void PnLEngine::onPriceUpdate(const std::string& symbol, double ltp) {
    std::lock_guard<std::mutex> lock(mutex_);
    ltps_[symbol] = ltp;
}

double PnLEngine::computeUnrealized(const Position& pos, double ltp) {
    if (pos.quantity == 0) return 0.0;
    return (ltp - pos.avgEntry) * pos.quantity;
}

PnLResult PnLEngine::computePnL() {
    std::lock_guard<std::mutex> lock(mutex_);
    double unreal = 0.0;
    for (const auto& [symbol, pos] : positions_) {
        if (ltps_.count(symbol))
            unreal += computeUnrealized(pos, ltps_[symbol]);
    }
    return {
        .unrealized = unreal,
        .realized = 0.0,  // Realized tracked in wallet for now
        .equity = wallet_ + unreal
    };
}
