#include <crow.h>  // If you're using Crow (lightweight HTTP server)
#include <spdlog/spdlog.h>
#include "redis_retry.hpp"

int startHealthCheckServer() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/healthz")
    ([] {
        redisContext* ctx = safeRedisConnect("127.0.0.1", 6379);
        if (!ctx || ctx->err) {
            SPDLOG_ERROR("Health check: Redis unavailable");
            if (ctx) redisFree(ctx);
            return crow::response(500, "Redis not OK");
        }
        redisFree(ctx);
        return crow::response(200, "OK");
    });

    SPDLOG_INFO("Starting /healthz endpoint on :8082");
    app.port(8082).run();
    return 0;
}
