#include "anomaly_detector.hpp"
#include "config_loader.hpp"
#include <hiredis/hiredis.h>
#include <chrono>
#include <ctime>
#include <sstream>
#include <spdlog/spdlog.h>

int anomaly_detected_total = 0;

AnomalyDetector::AnomalyDetector(boost::asio::io_context& io,
                                 std::shared_ptr<RuleEngine> ruleEngine,
                                 std::shared_ptr<AlertDispatcher> dispatcher)
    : io_(io), ruleEngine_(std::move(ruleEngine)), dispatcher_(std::move(dispatcher)) {
    timer_ = std::make_shared<boost::asio::steady_timer>(io_);
}

void AnomalyDetector::start() {
    poll();
}

void AnomalyDetector::poll() {
    try {
        auto host = getEnvVar("REDIS_HOST", "127.0.0.1");
        int port = std::stoi(getEnvVar("REDIS_PORT", "6379"));

        redisContext* ctx = redisConnect(host.c_str(), port);
        if (!ctx || ctx->err) {
            SPDLOG_ERROR("Redis connection failed: {}", ctx ? ctx->errstr : "null");
            return;
        }

        redisReply* reply = (redisReply*)redisCommand(ctx, "SMEMBERS users:set");
        std::vector<AlertContext> users;

        if (reply && reply->type == REDIS_REPLY_ARRAY) {
            auto now = std::chrono::system_clock::now();
            std::time_t t = std::chrono::system_clock::to_time_t(now);
            std::string timestamp = std::ctime(&t);
            timestamp.pop_back();

            for (size_t i = 0; i < reply->elements; ++i) {
                std::string userKey = "userctx:" + std::string(reply->element[i]->str);
                redisReply* data = (redisReply*)redisCommand(ctx, "HGETALL %s", userKey.c_str());
                if (!data || data->type != REDIS_REPLY_ARRAY) continue;

                AlertContext ctx;
                ctx.userId = reply->element[i]->str;
                ctx.timestamp = timestamp;

                for (size_t j = 0; j < data->elements; j += 2) {
                    std::string key = data->element[j]->str;
                    std::string val = data->element[j + 1]->str;
                    if (key == "symbol") ctx.symbol = val;
                    else if (key == "balance") ctx.balance = std::stod(val);
                    else if (key == "equity") ctx.equity = std::stod(val);
                    else if (key == "pnl") ctx.pnl = std::stod(val);
                    else if (key == "margin") ctx.margin = std::stod(val);
                }

                users.push_back(ctx);
                if (data) freeReplyObject(data);
            }
        }

        if (reply) freeReplyObject(reply);
        redisFree(ctx);

        for (auto& ctx : users) {
            auto alerts = ruleEngine_->evaluate(ctx);
            for (const auto& alert : alerts) {
                dispatcher_->dispatch(ctx.userId, alert);
            }
        }

        anomaly_detected_total++;

        timer_->expires_after(std::chrono::seconds(10));
        timer_->async_wait([this](auto) { poll(); });
    } catch (const std::exception& e) {
        SPDLOG_ERROR("AnomalyDetector poll error: {}", e.what());
    }
}

void AnomalyDetector::evaluate(const std::string& userId, const std::string& metric, double value) {
    SPDLOG_INFO("Evaluating anomaly for user: {}, metric: {}, value: {}", userId, metric, value);
    // Extend this later to real-time checks
}
