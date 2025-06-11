#include "redis_consumer.hpp"
#include "log_writer_async.hpp"
#include <spdlog/spdlog.h>
#include <rapidjson/document.h>
#include <thread>
#include <chrono>

RedisConsumer::RedisConsumer(const std::string& host, int port, const std::string& stream)
    : host_(host), port_(port), stream_(stream), running_(false), redis_(nullptr) {}

RedisConsumer::~RedisConsumer() {
    stop();
}

void RedisConsumer::start() {
    running_ = true;
    redis_ = redisConnect(host_.c_str(), port_);
    if (!redis_ || redis_->err) {
        SPDLOG_ERROR("Failed to connect to Redis: {}", redis_ ? redis_->errstr : "Unknown");
        return;
    }
    worker_ = std::thread(&RedisConsumer::run, this);
    SPDLOG_INFO("RedisConsumer started on stream: {}", stream_);
}

void RedisConsumer::stop() {
    running_ = false;
    if (worker_.joinable()) {
        worker_.join();
    }
    if (redis_) {
        redisFree(redis_);
        redis_ = nullptr;
    }
    SPDLOG_INFO("RedisConsumer stopped");
}

void RedisConsumer::run() {
    std::string lastId = "$";

    while (running_) {
        std::string cmd = "XREAD BLOCK 5000 COUNT 10 STREAMS " + stream_ + " " + lastId;
        redisReply* reply = (redisReply*)redisCommand(redis_, cmd.c_str());

        if (!reply || reply->type != REDIS_REPLY_ARRAY) {
            freeReplyObject(reply);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        for (size_t i = 0; i < reply->elements; ++i) {
            redisReply* streamData = reply->element[i];
            if (streamData->type != REDIS_REPLY_ARRAY || streamData->elements < 2)
                continue;

            redisReply* entries = streamData->element[1];
            for (size_t j = 0; j < entries->elements; ++j) {
                redisReply* entry = entries->element[j];
                if (entry->type != REDIS_REPLY_ARRAY || entry->elements < 2)
                    continue;

                lastId = entry->element[0]->str;

                redisReply* keyvals = entry->element[1];
                for (size_t k = 0; k + 1 < keyvals->elements; k += 2) {
                    std::string jsonStr = keyvals->element[k + 1]->str;
                    try {
                        rapidjson::Document d;
                        d.Parse(jsonStr.c_str());
                        if (d.HasParseError() || !d.HasMember("type")) {
                            SPDLOG_WARN("Invalid audit message: {}", jsonStr);
                            continue;
                        }

                        std::string type = d["type"].GetString();
                        if (type == "trade") {
                            TradeEvent e;
                            e.set_symbol(d["symbol"].GetString());
                            e.set_makerorderid(d["maker"].GetString());
                            e.set_takerorderid(d["taker"].GetString());
                            e.set_price(d["price"].GetDouble());
                            e.set_quantity(d["quantity"].GetDouble());
                            e.set_timestamp(d["timestamp"].GetString());
                            AsyncLogWriter::get().enqueue(e);
                        } else if (type == "order") {
                            OrderEvent e;
                            e.set_orderid(d["orderId"].GetString());
                            e.set_userid(d["userId"].GetString());
                            e.set_symbol(d["symbol"].GetString());
                            e.set_side(d["side"].GetString());
                            e.set_type_detail(d["type_detail"].GetString());
                            e.set_price(d["price"].GetDouble());
                            e.set_quantity(d["quantity"].GetDouble());
                            e.set_status(d["status"].GetString());
                            e.set_timestamp(d["timestamp"].GetString());
                            AsyncLogWriter::get().enqueue(e);
                        } else if (type == "funding") {
                            FundingEvent e;
                            e.set_symbol(d["symbol"].GetString());
                            e.set_fundingrate(d["rate"].GetDouble());
                            e.set_longoi(d["longOi"].GetDouble());
                            e.set_shortoi(d["shortOi"].GetDouble());
                            e.set_timestamp(d["timestamp"].GetString());
                            AsyncLogWriter::get().enqueue(e);
                        } else if (type == "liquidation") {
                            LiquidationEvent e;
                            e.set_userid(d["userId"].GetString());
                            e.set_symbol(d["symbol"].GetString());
                            e.set_pnl(d["pnl"].GetDouble());
                            e.set_reason(d["reason"].GetString());
                            e.set_timestamp(d["timestamp"].GetString());
                            AsyncLogWriter::get().enqueue(e);
                        } else {
                            SPDLOG_WARN("Unknown event type: {}", type);
                        }
                    } catch (const std::exception& e) {
                        SPDLOG_ERROR("RedisConsumer exception: {}", e.what());
                    }
                }
            }
        }

        freeReplyObject(reply);
    }
}
