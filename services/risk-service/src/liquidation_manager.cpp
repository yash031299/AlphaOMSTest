#include "liquidation_manager.hpp"
#include "position_tracker.hpp"
#include "redis_connection.hpp"
#include <spdlog/spdlog.h>

void LiquidationManager::liquidate(const std::string& userId,
                                   const std::string& symbol,
                                   double positionSize,
                                   int side,  // +1 for long, -1 for short
                                   double ltp) {
    SPDLOG_WARN("🔻 LIQUIDATION TRIGGERED: {} @ {}", userId, ltp);

    // 1. Update position as liquidated
    PositionTracker tracker;
    tracker.updateLiquidatedPosition(userId);

    // 2. Publish liquidation event to Redis
    RedisPublisher redis;
    redis.tradeMessage(userId, symbol, ltp, positionSize, (side == 1 ? "SELL" : "BUY"), "LIQUIDATION");

    // 3. Optional: Log to audit or DB
    SPDLOG_INFO("💥 Liquidation published for {} on {}", userId, symbol);
}
