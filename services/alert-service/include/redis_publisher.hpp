#pragma once
#include <string>
#include <memory>
#include <hiredis/hiredis.h>
#include "rule_engine.hpp"  // for Alert

class RedisPublisher {
public:
    RedisPublisher(const std::string& host = "127.0.0.1", int port = 6379);
    ~RedisPublisher();

    void publishAlert(const std::string& userId, const Alert& alert);

private:
    redisContext* context;
    std::string host_;
    int port_;
};
