#pragma once
#include <string>

class SLOrderExecutor {
public:
    SLOrderExecutor(const std::string& symbol);

    bool execute(double ltp, double stopPrice, double quantity, int side);  // 1 = buy SL, -1 = sell SL

private:
    std::string symbol_;
    double lastTriggeredPrice_;
};
