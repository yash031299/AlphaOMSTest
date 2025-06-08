#include "position_tracker.hpp"
#include <spdlog/spdlog.h>

void PositionTracker::updateLiquidatedPosition(const std::string& userId) {
    SPDLOG_INFO("Marking positions as liquidated for user: {}", userId);
    // Update Redis/DB logic goes here
}
