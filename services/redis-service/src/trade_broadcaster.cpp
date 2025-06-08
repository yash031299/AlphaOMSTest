#include "trade_broadcaster.hpp"
#include "redis_connection.hpp"
#include <spdlog/spdlog.h>

void TradeBroadcaster::publishTrade(const std::string& tradeJson) {
    RedisConnection redis;
    auto ctx = redis.get();
    if (!ctx) return;

    SPDLOG_INFO("Publishing trade: {}", tradeJson);
    redisCommand(ctx, "PUBLISH trades_channel %s", tradeJson.c_str());
}
