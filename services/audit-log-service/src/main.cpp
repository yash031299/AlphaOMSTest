#include "log_writer.hpp"
#include <spdlog/spdlog.h>
#include <thread>
#include <chrono>

int main() {
    spdlog::set_level(spdlog::level::info);
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%^%l%$] %v");

    LogWriter logger("data/logs");

    SPDLOG_INFO("🚀 Audit Log Service Started");

    TradeEvent trade{
        .symbol = "BTC-USDT",
        .makerOrderId = "m123",
        .takerOrderId = "t456",
        .price = 27350.5,
        .quantity = 0.5,
        .timestamp = "2025-06-07 22:00:00"
    };
    logger.writeTrade(trade);

    OrderEvent order{
        .orderId = "ord-789",
        .userId = "user-001",
        .symbol = "ETH-USDT",
        .side = "buy",
        .type = "limit",
        .price = 1830.0,
        .quantity = 2.0,
        .status = "filled",
        .timestamp = "2025-06-07 22:00:03"
    };
    logger.writeOrder(order);

    FundingEvent funding{
        .symbol = "BTC-USDT",
        .fundingRate = 0.0008,
        .longOi = 5800.0,
        .shortOi = 4200.0,
        .timestamp = "2025-06-07 22:00:10"
    };
    logger.writeFunding(funding);

    LiquidationEvent liq{
        .userId = "user-999",
        .symbol = "BTC-USDT",
        .pnl = -140.25,
        .reason = "Margin exhausted",
        .timestamp = "2025-06-07 22:00:30"
    };
    logger.writeLiquidation(liq);

    SPDLOG_INFO("✍️ Sample events logged to disk.");

    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 0;
}
