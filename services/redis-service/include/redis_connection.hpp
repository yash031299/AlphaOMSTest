#pragma once
#include <hiredis/hiredis.h>

class RedisConnection {
public:
    RedisConnection();
    ~RedisConnection();
    redisContext* get();
private:
    redisContext* ctx;
};
