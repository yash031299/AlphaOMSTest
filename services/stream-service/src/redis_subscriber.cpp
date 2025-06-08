#include "redis_subscriber.hpp"
#include "tick_parser.hpp"
#include <hiredis/hiredis.h>
#include <spdlog/spdlog.h>

void RedisSubscriber::subscribeToStream(const std::string& streamKey) {
    SPDLOG_INFO("Subscribing to Redis stream: {}", streamKey);
    redisContext* ctx = redisConnect("127.0.0.1", 6379);
    if (!ctx || ctx->err) {
        SPDLOG_ERROR("Redis connection error");
        return;
    }

    std::string lastId = "$";
    while (true) {
        redisReply* reply = (redisReply*)redisCommand(ctx, "XREAD BLOCK 5000 STREAMS %s %s",
                                                      streamKey.c_str(), lastId.c_str());
        if (reply && reply->type == REDIS_REPLY_ARRAY && reply->elements > 0) {
            SPDLOG_INFO("Received Redis stream data...");
            TickParser parser;
            parser.process(reply);
        }
        if (reply) freeReplyObject(reply);
    }

    redisFree(ctx);
}
