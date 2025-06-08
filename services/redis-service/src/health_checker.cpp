#include <crow.h>  // Minimalist C++ web server
#include <spdlog/spdlog.h>
#include "redis_connection.hpp"

int startHealthCheckServer() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/healthz")
    ([] {
        redisContext* ctx = safeRedisConnect("127.0.0.1", 6379);
        if (!ctx || ctx->err) {
            SPDLOG_ERROR("Health check: Redis not OK");
            if (ctx) redisFree(ctx);
            return crow::response(500, "Redis not OK");
        }

        redisFree(ctx);
        return crow::response(200, "OK");
    });

    SPDLOG_INFO("Starting redis-service /healthz at port 8084");
    app.port(8084).run();
    return 0;
}
