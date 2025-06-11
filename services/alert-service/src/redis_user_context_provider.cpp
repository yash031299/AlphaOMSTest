#include "user_context_provider.hpp"
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>
#include <hiredis/hiredis.h>

class RedisUserContextProvider : public UserContextProvider {
public:
    std::vector<AlertContext> fetchAllContexts() override {
        std::vector<AlertContext> result;

        redisContext* ctx = redisConnect("127.0.0.1", 6379);
        if (!ctx || ctx->err) {
            SPDLOG_ERROR("Failed to connect to Redis: {}", ctx ? ctx->errstr : "null");
            return result;
        }

        redisReply* reply = (redisReply*)redisCommand(ctx, "SMEMBERS users:set");
        if (reply && reply->type == REDIS_REPLY_ARRAY) {
            for (size_t i = 0; i < reply->elements; ++i) {
                std::string userKey = "userctx:" + std::string(reply->element[i]->str);
                redisReply* data = (redisReply*)redisCommand(ctx, "HGETALL %s", userKey.c_str());
                if (data && data->type == REDIS_REPLY_ARRAY) {
                    AlertContext ctxData;
                    ctxData.userId = reply->element[i]->str;
                    for (size_t j = 0; j < data->elements; j += 2) {
                        std::string key = data->element[j]->str;
                        std::string val = data->element[j + 1]->str;
                        if (key == "symbol") ctxData.symbol = val;
                        if (key == "balance") ctxData.balance = std::stod(val);
                        if (key == "equity") ctxData.equity = std::stod(val);
                        if (key == "pnl") ctxData.pnl = std::stod(val);
                        if (key == "margin") ctxData.margin = std::stod(val);
                    }
                    result.push_back(ctxData);
                }
                if (data) freeReplyObject(data);
            }
        }
        if (reply) freeReplyObject(reply);
        redisFree(ctx);
        return result;
    }
};