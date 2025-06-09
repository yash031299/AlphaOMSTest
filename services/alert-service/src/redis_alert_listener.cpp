#include "redis_alert_listener.hpp"
#include "alert_dispatcher.hpp"

#include <hiredis/hiredis.h>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <chrono>

extern std::shared_ptr<AlertDispatcher> dispatcher;

void RedisAlertListener::start() {
    running = true;
    listenerThread = std::thread(&RedisAlertListener::listen, this);
}

void RedisAlertListener::stop() {
    running = false;
    if (listenerThread.joinable()) {
        listenerThread.join();
    }
}

void RedisAlertListener::listen() {
    SPDLOG_INFO("📡 RedisAlertListener subscribing to CHANNEL:ALERTS...");

    redisContext* redis = redisConnect("127.0.0.1", 6379);
    if (!redis || redis->err) {
        SPDLOG_ERROR("Redis connection failed: {}", redis ? redis->errstr : "null");
        return;
    }

    redisReply* reply = nullptr;
    redisCommand(redis, "SUBSCRIBE CHANNEL:ALERTS");

    while (running) {
        if (redisGetReply(redis, (void**)&reply) != REDIS_OK || !reply) {
            SPDLOG_ERROR("Failed to get reply from Redis.");
            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;
        }

        if (reply->type == REDIS_REPLY_ARRAY && reply->elements == 3) {
            const char* raw = reply->element[2]->str;

            try {
                nlohmann::json j = nlohmann::json::parse(raw);
                std::string userId = j.value("userId", "unknown");
                std::string symbol = j.value("symbol", "N/A");
                std::string event = j.value("event", "UNKNOWN");

                std::string alert = fmt::format("[{}] ALERT for {}: {}", event, userId, symbol);
                dispatcher->dispatch(alert);
            } catch (const std::exception& e) {
                SPDLOG_ERROR("JSON parse error: {}", e.what());
            }
        }

        freeReplyObject(reply);
    }

    redisFree(redis);
}
