#pragma once
#include <string>

struct UserState {
    std::string userId;
    double entryPrice;
    double positionSize;
    int side;           // +1 (long), -1 (short)
    double leverage;
    double walletBalance;
};