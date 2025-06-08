#pragma once
#include "redis_update.h"
#include "user_manage.h"
#include <memory>
#include <string>
#include <atomic>
#include <thread>

class OrderRouterService {
public:
    OrderRouterService(std::shared_ptr<RedisConnectionPool> redis, std::shared_ptr<userManager> userMng);
    ~OrderRouterService();
    void start();
    void stop();

private:
    void listenLoop();
    void processOrder(const std::string& jsonString);

    std::shared_ptr<RedisConnectionPool> redisPool;
    std::shared_ptr<userManager> userMng;

    redisContext* redis;
    std::atomic<bool> running;
    std::thread listenerThread;
};
