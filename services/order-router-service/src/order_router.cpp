#include "order_router.hpp"
#include "grpc_client.hpp"
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

    if (doc.HasParseError() || !doc.HasMember("orderType") || !doc.HasMember("symbol")) {
        SPD_ERROR("Invalid order JSON: {}", jsonString);
        return;
    }

    const std::string orderType = doc["orderType"].GetString();
    const std::string symbol = doc["symbol"].GetString();

    if (orderType == "MARKET" || orderType == "SL") {
        SPD_INFO("Routing {} order to execution-service", orderType);
        try {
            ExecutionGRPCClient execClient(std::getenv("EXECUTION_HOST"));
            bool ok = execClient.sendOrder(jsonString);
            if (!ok) SPD_ERROR("execution-service gRPC returned failure");
        } catch (const std::exception& ex) {
            SPD_ERROR("gRPC call failed: {}", ex.what());
        }

    } else if (orderType == "LIMIT") {
        SPD_INFO("Routing LIMIT order to match-engine queue");
        std::string queue = "QUEUE:MATCH:" + symbol;
        redisCommand(redis, "RPUSH %s %s", queue.c_str(), jsonString.c_str());
    } else {
        SPD_WARN("Unknown orderType '{}', skipping", orderType);
    }
}
