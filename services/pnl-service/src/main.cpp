#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <thread>
#include <hiredis/hiredis.h>
#include <rapidjson/document.h>
#include "grpc_interface.hpp"

void streamTradeListener(PnLServiceImpl* service) {
    SPDLOG_INFO("🧠 Starting trade stream listener...");

    redisContext* redis = redisConnect("127.0.0.1", 6379);
    if (!redis || redis->err) {
        SPDLOG_ERROR("Redis connection failed: {}", redis ? redis->errstr : "null");
        return;
    }

    std::string last_id = "0";

    while (true) {
        redisReply* reply = (redisReply*)redisCommand(redis, "XREAD COUNT 1 BLOCK 0 STREAMS STREAM:TRADE %s", last_id.c_str());
        if (!reply || reply->type != REDIS_REPLY_ARRAY) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            continue;
        }

        if (reply->element[0]->elements >= 2) {
            auto entries = reply->element[0]->element[1];
            for (size_t i = 0; i < entries->elements; ++i) {
                auto entry = entries->element[i];
                std::string entry_id = entry->element[0]->str;
                std::string jsonTrade = entry->element[1]->element[1]->str;
                last_id = entry_id;

                rapidjson::Document doc;
                doc.Parse(jsonTrade.c_str());

                if (doc.HasParseError()) {
                    SPDLOG_ERROR("JSON parse error: {}", jsonTrade);
                    continue;
                }

                std::string userId = doc["user"].GetString();
                std::string symbol = doc["symbol"].GetString();
                double price = doc["price"].GetDouble();
                double qty = doc["qty"].GetDouble();
                std::string side = doc["side"].GetString();

                auto user = service->getOrCreateUser(userId);  // See below
                user->updateLTP(symbol, price);

                double signedQty = (side == "BUY") ? qty : -qty;
                user->updatePosition(symbol, signedQty, price);
            }
        }

        freeReplyObject(reply);
    }

    redisFree(redis);
}
