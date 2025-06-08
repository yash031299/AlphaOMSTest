#pragma once
#include <hiredis/hiredis.h>
#include <spdlog/spdlog.h>
#include <thread>
#include <chrono>

inline redisContext* safeRedisConnect(const std::string& host, int port, int maxRetries = 5) {
    redisContext* ctx = nullptr;
    int retryCount = 0;
    while (retryCount < maxRetries) {
        ctx = redisConnect(host.c_str(), port);
        if (ctx && !ctx->err) return ctx;

        SPDLOG_WARN("Redis connect retry {}/{} failed: {}", retryCount + 1, maxRetries, ctx ? ctx->errstr : "null");
        if (ctx) redisFree(ctx);

        std::this_thread::sleep_for(std::chrono::milliseconds(100 * (1 << retryCount)));
        retryCount++;
    }
    SPDLOG_ERROR("Redis connection failed after {} attempts", maxRetries);
    return nullptr;
}

inline redisReply* safeRedisCommand(redisContext* ctx, const char* format, ...) {
    if (!ctx) return nullptr;

    va_list args;
    va_start(args, format);
    redisReply* reply = (redisReply*)redisvCommand(ctx, format, args);
    va_end(args);

    if (!reply || reply->type == REDIS_REPLY_ERROR) {
        SPDLOG_ERROR("Redis command error: {}", reply ? reply->str : "no reply");
        if (reply) freeReplyObject(reply);
        return nullptr;
    }

    return reply;
}
