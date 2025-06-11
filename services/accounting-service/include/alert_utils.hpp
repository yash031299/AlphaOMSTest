// include/alert_utils.hpp
#pragma once

#include <string>
#include <sstream>
#include <hiredis/hiredis.h>
#include <spdlog/spdlog.h>

inline void publishAlert(const std::string& msg) {
    redisContext* ctx = redisConnect("localhost", 6379);
    if (!ctx || ctx->err) {
        SPDLOG_ERROR("❌ Redis connect error: {}", ctx ? ctx->errstr : "unknown");
        return;
    }

    std::stringstream alert;
    alert << R"({"type":"accounting_error","message":")" << msg << R"("})";
    redisCommand(ctx, "PUBLISH accounting-alerts %s", alert.str().c_str());
    redisFree(ctx);
}
