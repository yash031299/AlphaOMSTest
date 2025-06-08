#pragma once
#include <string>
#include "user_state.hpp" 

class RMSEngine {
public:
    RMSEngine(const UserState& state);

    void onPriceUpdate(double ltp);
    void evaluateRisk();
    void syncMargin();

    double getUnrealizedPnL() const { return unrealizedPnL_; }
    bool shouldLiquidate() const {
        double equity = state_.walletBalance + unrealizedPnL_;
        return equity < (marginUsed_ * 0.05);  // assuming 5% MM
    }


private:
    std::string userId_;
    double lastLTP_;
    double marginUsed_;
    double unrealizedPnL_;
    double liquidationPrice_;
    std::mutex lock_;
    UserState state_;


    void calculatePnL();
    void checkLiquidation();
};
