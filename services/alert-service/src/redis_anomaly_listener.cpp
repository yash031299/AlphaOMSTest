#include "redis_anomaly_listener.hpp"
#include "anomaly_detector.hpp"
#include <hiredis/hiredis.h>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <chrono>

extern std::shared_ptr<AnomalyDetector> anomalyDetector;

RedisAnomalyListener::RedisAnomalyListener(std::shared_ptr<AlertDispatcher> dispatcher)
    : dispatcher_(std::move(dispatcher)) {
    }


void RedisAnomalyListener::start() {
    running = true;
    listenerThread = std::thread(&RedisAnomalyListener::listen, this);
}

void RedisAnomalyListener::stop() {
    running = false;
    if (listenerThread.joinable()) {
        listenerThread.join();
    }
}

void RedisAnomalyListener::listen() {
    SPDLOG_INFO("📡 RedisAnomalyListener subscribing to CHANNEL:ANOMALY_EVENTS");

    redisContext* redis = redisConnect("127.0.0.1", 6379);
    if (!redis || redis->err) {
        SPDLOG_ERROR("Redis connection failed: {}", redis ? redis->errstr : "null");
        return;
    }

    redisReply* reply = nullptr;
    redisCommand(redis, "SUBSCRIBE CHANNEL:ANOMALY_EVENTS");

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
                std::string userId = j.at("userId");
                std::string metric = j.at("metric");
                double value = j.at("value");

                SPDLOG_INFO("🧠 Triggering anomaly check: {} {}={}", userId, metric, value);
                if (anomalyDetector) anomalyDetector->evaluate(userId, metric, value);

            } catch (const std::exception& e) {
                SPDLOG_ERROR("JSON parse error: {}", e.what());
            }
        }

        freeReplyObject(reply);
    }

    redisFree(redis);
}
