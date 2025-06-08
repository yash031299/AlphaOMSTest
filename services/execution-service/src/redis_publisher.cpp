#include "redis_publisher.hpp"
#include <sw/redis++/redis++.h>
#include <spdlog/spdlog.h>
using namespace sw::redis;


Redis redisClient("tcp://localhost:6379");

RedisPublisher::RedisPublisher() {}

void RedisPublisher::publishTrade(const std::string& symbol,
                                  const std::string& user_id,
                                  double price,
                                  double qty,
                                  const std::string& side) {
    try {
        std::string payload = fmt::format(R"({{"symbol":"{}","user":"{}","price":{},"qty":{},"side":"{}"}})",
                                          symbol, user_id, price, qty, side);

        redisClient.xadd("STREAM:TRADE", "*", {{"trade", payload}});
        SPDLOG_INFO("XADD trade to STREAM:TRADE: {}", payload);
    } catch (const std::exception& e) {
        SPDLOG_ERROR("Redis XADD failed: {}", e.what());
    }
}
