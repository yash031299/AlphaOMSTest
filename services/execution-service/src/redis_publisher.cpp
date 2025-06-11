#include "redis_publisher.hpp"
#include <sw/redis++/redis++.h>
#include <spdlog/spdlog.h>
#include <cstdlib>
#include <nlohmann/json.hpp>


using json = nlohmann::json;
using namespace sw::redis;

RedisPublisher::RedisPublisher() {
    try {
        std::string redis_url = std::getenv("REDIS_URL") ? std::getenv("REDIS_URL") : "tcp://localhost:6379";
        redis_ = std::make_shared<Redis>(redis_url);
        SPDLOG_INFO("RedisPublisher initialized with {}", redis_url);
    } catch (const std::exception& e) {
        SPDLOG_ERROR("Failed to connect to Redis: {}", e.what());
        redis_ = nullptr;
    }
}
void RedisPublisher::publishTrade(const order::OrderRequest& req, const std::string& trade_id) {
    try {
        json trade = {
            {"symbol", req.symbol()},
            {"user", req.user_id()},
            {"qty", req.quantity()},
            {"price", req.price()},
            {"side", req.side()},
            {"trade_id", trade_id}
        };
        std::string payload = trade.dump();

        for (int attempt = 1; attempt <= 3; ++attempt) {
            try {
                redis_->xadd("STREAM:TRADE", "*", std::initializer_list<std::pair<std::string, std::string>>{{"trade", payload}});
                SPDLOG_INFO("🧾 Trade published to STREAM:TRADE: {}", payload);
                return;
            } catch (const sw::redis::Error& e) {
                SPDLOG_WARN("⚠️ Redis publishTrade failed (attempt {}): {}", attempt, e.what());
                std::this_thread::sleep_for(std::chrono::milliseconds(100 * (1 << (attempt - 1))));
            }
        }

        SPDLOG_ERROR("❌ Failed to publish trade after retries: {}", payload);
    } catch (const std::exception& ex) {
        SPDLOG_ERROR("🔥 publishTrade exception: {}", ex.what());
    }
}

void RedisPublisher::publishReject(const order::OrderRequest& req, const std::string& reason) {
    try {
        json reject = {
            {"symbol", req.symbol()},
            {"user", req.user_id()},
            {"qty", req.quantity()},
            {"price", req.price()},
            {"side", req.side()},
            {"reason", reason}
        };
        std::string payload = reject.dump();

        for (int attempt = 1; attempt <= 3; ++attempt) {
            try {
                redis_->xadd("STREAM:REJECT", "*", std::initializer_list<std::pair<std::string, std::string>>{{"reject", payload}});
                SPDLOG_WARN("🚫 Order rejected and published to STREAM:REJECT: {}", payload);
                return;
            } catch (const sw::redis::Error& e) {
                SPDLOG_WARN("⚠️ Redis publishReject failed (attempt {}): {}", attempt, e.what());
                std::this_thread::sleep_for(std::chrono::milliseconds(100 * (1 << (attempt - 1))));
            }
        }

        SPDLOG_ERROR("❌ Failed to publish rejection after retries: {}", payload);
    } catch (const std::exception& ex) {
        SPDLOG_ERROR("🔥 publishReject exception: {}", ex.what());
    }
}

// Test Code for redis++
// redis_->xadd("STREAM:TRADE", "*", std::initializer_list<std::pair<std::string, std::string>>{{"trade", payload}});
// redis_->xadd("STREAM:REJECT", "*", std::initializer_list<std::pair<std::string, std::string>>{{"reject", payload}});