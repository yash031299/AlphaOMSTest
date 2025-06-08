#include "order_router.hpp"
#include "include/utils/logger.h"
#include <hiredis/hiredis.h>
#include <rapidjson/document.h>

OrderRouterService::OrderRouterService(std::shared_ptr<RedisConnectionPool> redis, std::shared_ptr<userManager> userMng)
    : redisPool(redis), userMng(userMng), running(false) {
    redis = redisConnect("127.0.0.1", 6379);
    if (!redis || redis->err) {
        SPD_CRITICAL("Redis connection error: {}", redis ? redis->errstr : "unknown");
        exit(1);
    }
}

OrderRouterService::~OrderRouterService() {
    stop();
    if (redis) redisFree(redis);
}

void OrderRouterService::start() {
    running = true;
    listenerThread = std::thread(&OrderRouterService::listenLoop, this);
}

void OrderRouterService::stop() {
    running = false;
    if (listenerThread.joinable()) listenerThread.join();
}

void OrderRouterService::listenLoop() {
    SPD_INFO("OrderRouterService: Listening to QUEUE:ORDER");
    while (running) {
        redisReply* reply = (redisReply*)redisCommand(redis, "BLPOP QUEUE:ORDER 5");
        if (!reply || reply->type != REDIS_REPLY_ARRAY || reply->elements < 2) {
            if (reply) freeReplyObject(reply);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        std::string jsonOrder(reply->element[1]->str);
        SPD_DEBUG("Received order: {}", jsonOrder);
        processOrder(jsonOrder);

        freeReplyObject(reply);
    }
}

void OrderRouterService::processOrder(const std::string& jsonString) {
    rapidjson::Document doc;
    doc.Parse(jsonString.c_str());

    if (doc.HasParseError() || !doc.HasMember("userId") || !doc.HasMember("symbol")) {
        SPD_ERROR("Invalid order JSON: {}", jsonString);
        return;
    }

    std::string userId = doc["userId"].GetString();
    std::string orderId = doc["orderId"].GetString();
    std::string orderType = doc["orderType"].GetString();

    SPD_INFO("Routing order {} of type {} for user {}", orderId, orderType, userId);

    redisPool->enqueueUpdate(jsonString); // Write to Redis
    // TODO: route to execution/match-engine (deferred to integration phase)
}
