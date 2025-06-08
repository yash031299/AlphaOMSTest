#include "redis_connection.hpp"
#include <spdlog/spdlog.h>

RedisConnection::RedisConnection() {
    ctx = redisConnect("127.0.0.1", 6379);
    if (!ctx || ctx->err) {
        SPDLOG_ERROR("Failed to connect to Redis");
        ctx = nullptr;
    }
}

RedisConnection::~RedisConnection() {
    if (ctx) redisFree(ctx);
}

redisContext* RedisConnection::get() {
    return ctx;
}
