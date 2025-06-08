#include "margin_listener.hpp"
#include "rms_engine.hpp"
#include "redis_connection.hpp"
#include <hiredis/hiredis.h>
#include <spdlog/spdlog.h>
#include <chrono>

extern std::unordered_map<std::string, std::shared_ptr<RiskManagement>> rmsMap;

void MarginListener::start() {
    running = true;
    listenerThread = std::thread(&MarginListener::listen, this);
}

void MarginListener::stop() {
    running = false;
    if (listenerThread.joinable()) {
        listenerThread.join();
    }
}

void MarginListener::listen() {
    SPDLOG_INFO("📥 Starting MarginListener on QUEUE:MARGIN:CHECK:*");

    redisContext* redis = redisConnect("127.0.0.1", 6379);
    if (!redis || redis->err) {
        SPDLOG_ERROR("Redis connection failed: {}", redis ? redis->errstr : "null");
        return;
    }

    while (running) {
        redisReply* reply = (redisReply*)redisCommand(redis, "BLPOP QUEUE:MARGIN:CHECK:* 0");

        if (!reply || reply->type != REDIS_REPLY_ARRAY || reply->elements < 2) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            continue;
        }

        std::string userId = reply->element[1]->str;
        SPDLOG_INFO("🧠 Received margin check trigger for user: {}", userId);

        if (rmsMap.count(userId)) {
            auto rms = rmsMap[userId];
            rms->syncMargin();  // Re-evaluate based on wallet state
            rms->evaluateRisk();
        } else {
            SPDLOG_WARN("RMS not initialized for user: {}", userId);
        }

        freeReplyObject(reply);
    }

    redisFree(redis);
}
