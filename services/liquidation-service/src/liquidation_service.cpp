#include "liquidation_service.hpp"
#include "include/utils/logger.h"
#include <iostream>
#include <sstream>
#include <thread>

LiquidationService::LiquidationService(std::shared_ptr<RedisConnectionPool> redisPool, std::shared_ptr<userManager> usrMng)
    : redisPool(redisPool), usrMng(usrMng), running(false) {
    redis = redisConnect("127.0.0.1", 6379);
    if (!redis || redis->err) {
        SPD_CRITICAL("Redis connection error: {}", redis ? redis->errstr : "unknown");
        exit(1);
    }
}

LiquidationService::~LiquidationService() {
    stop();
    if (redis) redisFree(redis);
}

void LiquidationService::start() {
    running = true;
    listenerThread = std::thread(&LiquidationService::listenLoop, this);
}

void LiquidationService::stop() {
    running = false;
    if (listenerThread.joinable()) listenerThread.join();
}

void LiquidationService::listenLoop() {
    SPD_INFO("LiquidationService: Started listening to LIQUIDATE:* keys");
    while (running) {
        redisReply* reply = (redisReply*)redisCommand(redis, "BLPOP QUEUE:LIQUIDATE 5");
        if (!reply || reply->type != REDIS_REPLY_ARRAY || reply->elements < 2) {
            if (reply) freeReplyObject(reply);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        std::string key(reply->element[1]->str);
        SPD_INFO("Received liquidation key: {}", key);
        processKey(key);
        freeReplyObject(reply);
    }
}

void LiquidationService::processKey(const std::string& key) {
    std::string redisKey = "USER:" + key;
    redisContext* conn = redisConnect("127.0.0.1", 6379);
    if (!conn || conn->err) return;

    redisReply* reply = (redisReply*)redisCommand(conn, "HGETALL %s", redisKey.c_str());
    if (!reply || reply->type != REDIS_REPLY_ARRAY) {
        if (reply) freeReplyObject(reply);
        redisFree(conn);
        return;
    }

    user usr;
    for (size_t i = 0; i < reply->elements; i += 2) {
        std::string field = reply->element[i]->str;
        std::string value = reply->element[i + 1]->str;
        if (field == "userid") usr.userid = value;
        else if (field == "wallet") usr.wallet = std::stod(value);
    }

    freeReplyObject(reply);
    redisFree(conn);

    SPD_INFO("Calling liquidation on user: {}", usr.userid);
    usrMng->liquidation(usr);
}
