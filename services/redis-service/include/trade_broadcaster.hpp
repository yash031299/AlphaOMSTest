#pragma once
#include <string>

class TradeBroadcaster {
public:
    void publishTrade(const std::string& tradeJson);
};
