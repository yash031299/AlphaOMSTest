#include "redis_alert_listener.hpp"
#include "alert_dispatcher.hpp"
#include "config_loader.hpp"
#include <hiredis/hiredis.h>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <thread>
#include <chrono>

void RedisAlertListener::start(std::shared_ptr<AlertDispatcher> dispatcher) {
    std::thread([dispatcher]() {
        try {
            std::string host = getEnvVar("REDIS_HOST", "127.0.0.1");
            int port = std::stoi(getEnvVar("REDIS_PORT", "6379"));

            redisContext* context = redisConnect(host.c_str(), port);
            if (!context || context->err) {
                SPDLOG_ERROR("Redis connection failed: {}", context ? context->errstr : "null");
                return;
            }

            redisReply* reply = (redisReply*)redisCommand(context, "SUBSCRIBE alert-event");
            if (reply) freeReplyObject(reply);

            while (true) {
                redisReply* msg = nullptr;
                if (redisGetReply(context, (void**)&msg) != REDIS_OK || !msg) {
                    SPDLOG_WARN("Redis alert-event subscription failed. Retrying...");
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    continue;
                }

                if (msg->type == REDIS_REPLY_ARRAY && msg->elements == 3) {
                    try {
                        std::string payload = msg->element[2]->str;
                        auto json = nlohmann::json::parse(payload);

                        std::string userId = json.value("userId", "");
                        std::string metric = json.value("metric", "");
                        double value = json.value("value", 0.0);
                        double threshold = json.value("threshold", 0.0);
                        std::string severityStr = json.value("severity", "LOW");
                        std::string message = json.value("message", "");
                        std::string timestamp = json.value("timestamp", "");

                        Alert alert;
                        alert.metric = metric;
                        alert.value = value;
                        alert.threshold = threshold;
                        alert.severity = from_string(severityStr);
                        alert.message = message;
                        alert.timestamp = timestamp;

                        dispatcher->dispatch(userId, alert);

                    } catch (const std::exception& e) {
                        SPDLOG_ERROR("Alert parse error: {}", e.what());
                    }
                }

                if (msg) freeReplyObject(msg);
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }

            redisFree(context);
        } catch (const std::exception& e) {
            SPDLOG_ERROR("RedisAlertListener thread error: {}", e.what());
        }
    }).detach();
}
