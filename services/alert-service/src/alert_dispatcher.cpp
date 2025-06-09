#include "alert_dispatcher.hpp"
#include <spdlog/spdlog.h>
#include "redis_publisher.hpp"
static std::unique_ptr<RedisPublisher> publisher = std::make_unique<RedisPublisher>();

void AlertDispatcher::dispatch(const std::string& userId, const Alert& alert) {
    SPDLOG_WARN("🚨 Alert for {} | {}: {} > {}", userId, alert.metric, alert.value, alert.threshold);
    publisher->publishAlert(userId, alert);
}
