#include <crow.h>
#include <spdlog/spdlog.h>
#include <hiredis/hiredis.h>

int startHealthCheckServer() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/healthz")
    ([] {
        redisContext* ctx = redisConnect("127.0.0.1", 6379);
        if (!ctx || ctx->err) {
            SPDLOG_ERROR("Health check: Redis unavailable");
            if (ctx) redisFree(ctx);
            return crow::response(500, "Redis not OK");
        }

        redisFree(ctx);
        return crow::response(200, "OK");
    });

    SPDLOG_INFO("[wallet-sync-service] /healthz available on port 8086");
    app.port(8086).run();
    return 0;
}
