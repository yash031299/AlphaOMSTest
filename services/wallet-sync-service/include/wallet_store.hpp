#pragma once
#include <string>

class WalletStore {
public:
    static void applyTrade(const std::string& userId, double pnl);
};
