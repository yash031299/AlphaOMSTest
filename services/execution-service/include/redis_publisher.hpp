#pragma once
#include <string>

class RedisPublisher {
public:
    explicit RedisPublisher();
    void publishTrade(const std::string& symbol,
                      const std::string& user_id,
                      double price,
                      double qty,
                      const std::string& side);
};
