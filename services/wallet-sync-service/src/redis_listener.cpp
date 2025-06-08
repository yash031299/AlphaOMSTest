#include "redis_listener.hpp"
#include "wallet_store.hpp"
#include <hiredis/hiredis.h>
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void RedisTradeListener::start(const std::string& host, int port, const std::string& channel) {
    redisContext* ctx = redisConnect(host.c_str(), port);
    if (!ctx || ctx->err) {
        SPDLOG_ERROR("Redis connection failed: {}", ctx ? ctx->errstr : "null");
        return;
    }

    redisReply* reply = (redisReply*)redisCommand(ctx, "SUBSCRIBE %s", channel.c_str());
    if (reply) freeReplyObject(reply);

    SPDLOG_INFO("Subscribed to Redis channel: {}", channel);

    while (redisGetReply(ctx, (void**)&reply) == REDIS_OK) {
        if (reply && reply->type == REDIS_REPLY_ARRAY && reply->elements == 3) {
            std::string payload = reply->element[2]->str;

            try {
                auto j = json::parse(payload);
                std::string userId = j.at("userId");
                double pnl = j.at("unrealizedPnL");

                WalletStore::applyTrade(userId, pnl);
            } catch (const std::exception& ex) {
                SPDLOG_ERROR("Invalid trade JSON: {}", ex.what());
            }
        }
        if (reply) freeReplyObject(reply);
    }

    redisFree(ctx);
}
