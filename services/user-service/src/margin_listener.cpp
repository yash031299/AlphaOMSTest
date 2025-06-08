#include "margin_listener.hpp"
#include <spdlog/spdlog.h>
#include <hiredis/hiredis.h>
#include <sstream>

MarginEventListener::MarginEventListener(std::shared_ptr<RedisSafeClient> redis,
                                         std::shared_ptr<UserWalletStore> walletStore)
    : redis_(std::move(redis)), walletStore_(std::move(walletStore)) {}

void MarginEventListener::start() {
    running_ = true;
    thread_ = std::thread([this]() {
        redisContext* ctx = redisConnect("127.0.0.1", 6379);
        if (!ctx || ctx->err) {
            SPDLOG_ERROR("MarginListener Redis connect error: {}", ctx ? ctx->errstr : "null");
            return;
        }

        redisReply* subReply = (redisReply*)redisCommand(ctx, "SUBSCRIBE MARGIN_UPDATES");
        freeReplyObject(subReply);

        SPDLOG_INFO("Subscribed to MARGIN_UPDATES");

        while (running_) {
            redisReply* reply;
            if (redisGetReply(ctx, (void**)&reply) == REDIS_OK && reply) {
                if (reply->type == REDIS_REPLY_ARRAY && reply->elements == 3) {
                    std::string message = reply->element[2]->str;
                    size_t sep = message.find(':');
                    if (sep != std::string::npos) {
                        std::string userId = message.substr(0, sep);
                        double delta = std::stod(message.substr(sep + 1));
                        walletStore_->applyDelta(userId, delta);
                    }
                }
                freeReplyObject(reply);
            }
        }

        redisFree(ctx);
    });
}

void MarginEventListener::stop() {
    running_ = false;
    if (thread_.joinable()) thread_.join();
}
