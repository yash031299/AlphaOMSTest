#pragma once
#include <string>

class RedisPublisher {
public:
    RedisPublisher();
    void tradeMessage(const std::string& user_id,
                      const std::string& symbol,
                      double price,
                      double quantity,
                      const std::string& side,
                      const std::string& type = "NORMAL");

private:
    redisContext* ctx_;
};
