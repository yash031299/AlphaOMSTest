#ifndef EQUITY_CALCULATOR_HPP
#define EQUITY_CALCULATOR_HPP

#include "pnl_engine.hpp"
#include "symbol_service.hpp"  // gRPC client to get margin data
#include <string>
#include <vector>
#include <memory>

class EquityCalculator {
public:
    EquityCalculator(const std::string& userId, double wallet);

    void setSymbolService(std::shared_ptr<SymbolServiceClient> symbolClient);

    void updateLTP(const std::string& symbol, double ltp);
    void updatePosition(const std::string& symbol, double qty, double entryPrice);

    double getEquity();
    bool isLiquidatable();

private:
    std::string userId_;
    PnLEngine engine_;
    std::shared_ptr<SymbolServiceClient> symbolClient_;
};

#endif // EQUITY_CALCULATOR_HPP
