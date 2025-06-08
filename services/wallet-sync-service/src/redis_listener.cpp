#include "redis_listener.hpp"
#include "wallet_store.hpp"
#include <hiredis/hiredis.h>
#include <spdlog/spdlog.h>
#include <rapidjson/document.h>
#include <thread>
#include <chrono>

extern WalletStore walletStore;

void RedisListener::start() {
    running = true;
    listenerThread = std::thread(&RedisListener::listen, this);
}

void RedisListener::stop() {
    running = false;
    if (listenerThread.joinable()) {
        listenerThread.join();
    }
}

void RedisListener::publishMarginCheck(const std::string& userId) {
    redisContext* redis = redisConnect("127.0.0.1", 6379);
    if (!redis || redis->err) {
        SPDLOG_ERROR("Redis connection error in publishMarginCheck: {}", redis->errstr);
        return;
    }

    std::string queue = "QUEUE:MARGIN:CHECK:" + userId;
    redisCommand(redis, "RPUSH %s %s", queue.c_str(), userId.c_str());

    redisFree(redis);
}

void RedisListener::listen() {
    redisContext* redis = redisConnect("127.0.0.1", 6379);
    if (!redis || redis->err) {
        SPDLOG_ERROR("Redis connection failed: {}", redis->errstr);
        return;
    }

    SPDLOG_INFO("📥 Wallet-Sync listening to STREAM:TRADE");

    std::string last_id = "0";

    while (running) {
        redisReply* reply = (redisReply*)redisCommand(redis, "XREAD COUNT 1 BLOCK 0 STREAMS STREAM:TRADE %s", last_id.c_str());

        if (!reply || reply->type != REDIS_REPLY_ARRAY) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        if (reply->element[0]->elements >= 2) {
            auto entries = reply->element[0]->element[1];
            for (size_t i = 0; i < entries->elements; ++i) {
                auto entry = entries->element[i];
                std::string entry_id = entry->element[0]->str;
                std::string tradeJson = entry->element[1]->element[1]->str;
                last_id = entry_id;

                walletStore.applyTrade(tradeJson);

                // extract user ID for margin check
                rapidjson::Document doc;
                doc.Parse(tradeJson.c_str());

                if (!doc.HasParseError() && doc.HasMember("user")) {
                    std::string userId = doc["user"].GetString();
                    publishMarginCheck(userId);
                }
            }
        }

        freeReplyObject(reply);
    }

    redisFree(redis);
}
