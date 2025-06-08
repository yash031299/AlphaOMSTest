#include "redis_connection.hpp"
#include "redis_retry.hpp"
#include <ctime>
#include <fmt/core.h>
#include <spdlog/spdlog.h>

RedisPublisher::RedisPublisher() {
    ctx_ = safeRedisConnect("127.0.0.1", 6379);
}

void RedisPublisher::tradeMessage(const std::string& user_id,
                                  const std::string& symbol,
                                  double price,
                                  double quantity,
                                  const std::string& side,
                                  const std::string& type) {
    if (!ctx_) return;

    std::string payload = fmt::format(R"({{"user_id":"{}","symbol":"{}","price":{},"quantity":{},"side":"{}","type":"{}","timestamp":{}}})",
                                      user_id, symbol, price, quantity, side, type, std::time(nullptr));

    redisReply* reply = safeRedisCommand(ctx_, "PUBLISH trades %s", payload.c_str());
    if (reply) {
        freeReplyObject(reply);
        SPDLOG_INFO("Published trade message: {}", payload);
    }
}
