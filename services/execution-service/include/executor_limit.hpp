#pragma once
#include <string>

class LimitOrderExecutor {
public:
    LimitOrderExecutor(const std::string& symbol);
    bool execute(double price, double quantity, int side);  // side: 1 = buy, -1 = sell

private:
    std::string symbol_;
    double lastExecutedPrice_;
};
