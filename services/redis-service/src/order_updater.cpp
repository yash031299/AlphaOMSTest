#include "order_updater.hpp"
#include "redis_connection.hpp"
#include <spdlog/spdlog.h>

void OrderUpdater::updateStatus(const std::string& orderId, const std::string& status) {
    RedisConnection redis;
    auto ctx = redis.get();
    if (!ctx) return;

    SPDLOG_INFO("Updating order {} with status {}", orderId, status);
    redisCommand(ctx, "HSET order:%s status %s", orderId.c_str(), status.c_str());
}
