#pragma once
#include <string>
#include <memory>
#include <atomic>
#include <thread>
#include <hiredis/hiredis.h>
#include "user_manage.h"

class LiquidationService {
public:
    LiquidationService(std::shared_ptr<RedisConnectionPool> redisPool, std::shared_ptr<userManager> usrMng);
    ~LiquidationService();
    void start();
    void stop();

private:
    void listenLoop();
    void processKey(const std::string& key);

    std::atomic<bool> running;
    std::thread listenerThread;
    redisContext* redis;
    std::shared_ptr<RedisConnectionPool> redisPool;
    std::shared_ptr<userManager> usrMng;
};
