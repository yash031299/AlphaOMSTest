#pragma once
#include <string>

class LiquidationManager {
public:
    void liquidate(const std::string& userId,
                   const std::string& symbol,
                   double positionSize,
                   int side,
                   double ltp);
};
