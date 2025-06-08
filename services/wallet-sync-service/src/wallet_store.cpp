#include "wallet_store.hpp"
#include <hiredis/hiredis.h>
#include <spdlog/spdlog.h>
#include "redis_retry.hpp"

void WalletStore::applyTrade(const std::string& userId, double pnl) {
    redisContext* ctx = safeRedisConnect("127.0.0.1", 6379);
    if (!ctx || ctx->err) {
        SPDLOG_ERROR("Wallet update failed: Redis unavailable for {}", userId);
        return;
    }

    redisReply* reply = safeRedisCommand(ctx,
        "HINCRBYFLOAT wallet:%s balance %f", userId.c_str(), pnl);

    if (reply) {
        SPDLOG_INFO("Wallet updated for {} → PnL: {:.2f}", userId, pnl);
        freeReplyObject(reply);
    }

    redisFree(ctx);
}
