#include "redis_listener.hpp"
#include <spdlog/spdlog.h>
#include <rapidjson/document.h>
#include <sstream>
#include <unistd.h>

RedisListener::RedisListener(const std::string& host, int port,
                             std::shared_ptr<Ledger> ledger)
    : ledger_(ledger), redis_ctx_(nullptr), running_(false) {
    redis_ctx_ = redisConnect(host.c_str(), port);
    if (!redis_ctx_ || redis_ctx_->err) {
        SPDLOG_CRITICAL("Redis connection failed: {}", redis_ctx_ ? redis_ctx_->errstr : "unknown");
        throw std::runtime_error("Redis connect failed");
    }
    fundingHandler_ = std::make_unique<FundingHandler>(ledger_);
    pnlTracker_ = std::make_unique<PnLTracker>(ledger_);
}

RedisListener::~RedisListener() {
    stop();
    if (redis_ctx_) redisFree(redis_ctx_);
}

void RedisListener::start() {
    running_ = true;
    listener_thread_ = std::thread(&RedisListener::listenLoop, this);
}

void RedisListener::stop() {
    running_ = false;
    if (listener_thread_.joinable())
        listener_thread_.join();
}

void RedisListener::listenLoop() {
    SPDLOG_INFO("📡 Listening to Redis stream: accounting-events");
    while (running_) {
        redisReply* reply;
        reply = (redisReply*)redisCommand(redis_ctx_,
            "XREAD COUNT 10 BLOCK 5000 STREAMS accounting-events $");

        if (!reply || reply->type != REDIS_REPLY_ARRAY) {
            usleep(500000); // backoff
            continue;
        }

        for (size_t i = 0; i < reply->elements; ++i) {
            redisReply* stream = reply->element[i];
            if (stream->elements < 2) continue;

            redisReply* messages = stream->element[1];
            for (size_t j = 0; j < messages->elements; ++j) {
                redisReply* msg = messages->element[j];
                std::string id = msg->element[0]->str;
                std::string json = msg->element[1]->element[1]->str;

                std::lock_guard<std::mutex> lock(id_mutex_);
                if (seen_ids_.count(id)) continue; // replay protection
                seen_ids_.insert(id);

                handleMessage(json);
            }
        }

        freeReplyObject(reply);
    }
}

void RedisListener::handleMessage(const std::string& json) {
    rapidjson::Document d;
    d.Parse(json.c_str());

    if (d.HasParseError() || !d.HasMember("type") || !d.HasMember("userId"))
        return;

    std::string type = d["type"].GetString();

    if (type == "funding") {
        fundingHandler_->applyFunding(
            d["userId"].GetString(), d["symbol"].GetString(),
            d["amount"].GetDouble(), d["timestamp"].GetString(),
            d["reference"].GetString());
    } else if (type == "pnl") {
        pnlTracker_->recordRealizedPnL(
            d["userId"].GetString(), d["symbol"].GetString(),
            d["amount"].GetDouble(), d["timestamp"].GetString(),
            d["reference"].GetString());
    } else {
        SPDLOG_WARN("Unknown accounting event type: {}", type);
    }
}
