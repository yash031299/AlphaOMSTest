#include "redis_publisher.hpp"
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <chrono>

RedisPublisher::RedisPublisher(const std::string& host, int port) {
    context = redisConnect(host.c_str(), port);
    if (!context || context->err) {
        SPDLOG_ERROR("Redis connection failed: {}", context ? context->errstr : "null");
        context = nullptr;
    }
}

RedisPublisher::~RedisPublisher() {
    if (context) redisFree(context);
}

void RedisPublisher::publishAlert(const std::string& userId, const Alert& alert) {
    if (!context) return;

    try {
        auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        nlohmann::json j = {
            {"userId", userId},
            {"metric", alert.metric},
            {"value", alert.value},
            {"threshold", alert.threshold},
            {"severity", alert.severity},
            {"message", alert.message},
            {"timestamp", now}
        };

        std::string payload = j.dump();
        redisReply* reply = (redisReply*)redisCommand(context, "PUBLISH alert-stream %s", payload.c_str());

        if (reply) {
            SPDLOG_INFO("📤 Published alert to Redis: {}", payload);
            freeReplyObject(reply);
        } else {
            SPDLOG_ERROR("Redis publish failed");
        }
    } catch (const std::exception& e) {
        SPDLOG_ERROR("Redis publishAlert error: {}", e.what());
    }
}
