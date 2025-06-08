#include "rms_engine.hpp"
#include <spdlog/spdlog.h>
#include <mutex>
#include <cmath>

namespace {
    std::mutex rms_mutex;  // Will later replace with finer-grained locking
}

RMSEngine::RMSEngine(const UserState& state)
    : state_(state), lastLTP_(0.0), marginUsed_(0.0), unrealizedPnL_(0.0), liquidationPrice_(0.0) {}


void RMSEngine::onPriceUpdate(double ltp) {
    std::lock_guard<std::mutex> guard(lock_);
    lastLTP_ = ltp;

    // 🔄 Fetch dynamic state
    redisContext* ctx = safeRedisConnect("127.0.0.1", 6379);
    if (!ctx || ctx->err) {
        SPDLOG_ERROR("Redis state fetch failed: {}", ctx ? ctx->errstr : "null ctx");
        return;
    }

    // Pull from Redis: rms:user-001:state
    redisReply* reply = safeRedisCommand(ctx,
        "HMGET rms:%s:state entryPrice positionSize side leverage walletBalance", state_.userId.c_str());

    if (reply && reply->type == REDIS_REPLY_ARRAY && reply->elements == 5) {
        state_.entryPrice     = std::stod(reply->element[0]->str);
        state_.positionSize   = std::stod(reply->element[1]->str);
        state_.side           = std::stoi(reply->element[2]->str);
        state_.leverage       = std::stod(reply->element[3]->str);
        state_.walletBalance  = std::stod(reply->element[4]->str);
    } else {
        SPDLOG_ERROR("Failed to parse user state from Redis");
    }

    if (reply) freeReplyObject(reply);
    redisFree(ctx);

    // Now safe to compute
    calculatePnL();
    evaluateRisk();
}


void RMSEngine::calculatePnL() {
    double notional = state_.entryPrice * state_.positionSize;
    double margin = notional / state_.leverage;

    // Correct formula (side is +1 for long, -1 for short)
    unrealizedPnL_ = state_.positionSize * (lastLTP_ - state_.entryPrice) * state_.side;
    marginUsed_ = margin;

    SPDLOG_INFO("User {} → PnL: {:.2f}, Margin Used: {:.2f}", state_.userId, unrealizedPnL_, marginUsed_);
}



void RMSEngine::evaluateRisk() {
    double maintMarginRate = 0.05;  // 5% maintenance margin

    double equity = state_.walletBalance + unrealizedPnL_;
    double liquidationThreshold = marginUsed_ * maintMarginRate;

    SPDLOG_INFO("Equity: {:.2f}, Liquidation Threshold: {:.2f}", equity, liquidationThreshold);

    if (equity < liquidationThreshold) {
        checkLiquidation();
    } else {
        SPDLOG_INFO("User {} passed RMS check ✅", state_.userId);
    }

    RMSMetrics::get().reportEquity(state_.userId, equity);
}



void RMSEngine::checkLiquidation() {
    SPDLOG_WARN("[RMS] User {} meets liquidation condition!", state_.userId);

    // 👇 New delegation to liquidation manager
    LiquidationManager manager;
    manager.liquidate(state_.userId,
                      state_.symbol,
                      state_.positionSize,
                      state_.side,
                      lastLTP_);

    RMSMetrics::get().incrementLiquidation();
}



void RMSEngine::syncMargin() {
    redisContext* ctx = safeRedisConnect("127.0.0.1", 6379);
    if (!ctx || ctx->err) {
        SPDLOG_ERROR("Redis sync failed for {}: {}", state_.userId, ctx ? ctx->errstr : "null");
        return;
    }

    std::string key = "rms:" + state_.userId;
    redisReply* reply = safeRedisCommand(ctx,
        "HMSET %s ltp %f pnl %f margin %f equity %f",
        key.c_str(), lastLTP_, unrealizedPnL_, marginUsed_, (unrealizedPnL_ + state_.walletBalance)
    );

    if (reply) freeReplyObject(reply);
    redisFree(ctx);

    SPDLOG_INFO("[RMS] Synced state for {} to Redis", state_.userId);
    RMSMetrics::get().reportEquity(state_.userId, state_.walletBalance + unrealizedPnL_);
}


