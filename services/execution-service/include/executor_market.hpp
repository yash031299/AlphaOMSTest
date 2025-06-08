#pragma once
#include <string>

class MarketOrderExecutor {
public:
    MarketOrderExecutor(const std::string& symbol);

    bool execute(double ltp, double quantity, int side);  // 1 = buy, -1 = sell

private:
    std::string symbol_;
    double lastExecutedPrice_;
};
