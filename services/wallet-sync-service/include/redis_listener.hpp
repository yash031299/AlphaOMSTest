#pragma once
#include <string>

class RedisTradeListener {
public:
    void start(const std::string& host, int port, const std::string& channel);
};
