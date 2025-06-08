#include "redis_safe_client.hpp"
#include <spdlog/spdlog.h>
#include <cstdarg>

RedisSafeClient::RedisSafeClient(const std::string& host, int port)
    : host_(host), port_(port), ctx_(nullptr) {
    connect();
}

RedisSafeClient::~RedisSafeClient() {
    if (ctx_) redisFree(ctx_);
}

bool RedisSafeClient::connect() {
    if (ctx_) redisFree(ctx_);
    ctx_ = redisConnect(host_.c_str(), port_);
    if (!ctx_ || ctx_->err) {
        SPDLOG_ERROR("Redis connect failed: {}", ctx_ ? ctx_->errstr : "nullptr");
        return false;
    }
    SPDLOG_INFO("Connected to Redis at {}:{}", host_, port_);
    return true;
}

bool RedisSafeClient::isConnected() const {
    return ctx_ && ctx_->err == 0;
}

redisReply* RedisSafeClient::command(const char* format, ...) {
    if (!isConnected()) {
        SPDLOG_WARN("Redis not connected, attempting reconnect...");
        if (!connect()) return nullptr;
    }

    va_list args;
    va_start(args, format);
    redisReply* reply = (redisReply*)redisvCommand(ctx_, format, args);
    va_end(args);

    if (!reply) {
        SPDLOG_ERROR("Redis command failed");
    }

    return reply;
}
