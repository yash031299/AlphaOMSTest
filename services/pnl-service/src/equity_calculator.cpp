#include "equity_calculator.hpp"
#include <spdlog/spdlog.h>

EquityCalculator::EquityCalculator(const std::string& userId, double wallet)
    : userId_(userId), engine_(userId, wallet) {}

void EquityCalculator::setSymbolService(std::shared_ptr<SymbolServiceClient> symbolClient) {
    symbolClient_ = std::move(symbolClient);
}

void EquityCalculator::updateLTP(const std::string& symbol, double ltp) {
    engine_.onPriceUpdate(symbol, ltp);
}

void EquityCalculator::updatePosition(const std::string& symbol, double qty, double entryPrice) {
    engine_.updatePosition(symbol, qty, entryPrice);
}

double EquityCalculator::getEquity() {
    return engine_.computePnL().equity;
}

bool EquityCalculator::isLiquidatable() {
    // 🔒 For now, we assume one symbol per user
    auto pnl = engine_.computePnL();

    try {
        auto meta = symbolClient_->getSymbolMetadata("BTC-USDT");
        double margin = meta.maintenance_margin();
        return pnl.equity < margin;
    } catch (const std::exception& e) {
        SPDLOG_ERROR("Symbol metadata failed: {}", e.what());
        return false;
    }
}
