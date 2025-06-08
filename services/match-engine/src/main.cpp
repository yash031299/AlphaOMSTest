#include "matcher.hpp"
#include "trade_publisher.hpp"
#include <spdlog/spdlog.h>
#include <thread>
#include <chrono>

int main() {
    auto logger = spdlog::stdout_color_mt("match-engine");
    spdlog::set_level(spdlog::level::info);
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%^%l%$] %v");

    SPDLOG_INFO("🚀 Starting Match Engine...");

    Matcher matcher;
    TradePublisher publisher;

    Order order1{ "order-001", "user-A", 101.0, 5.0, true };  // buy
    order1.symbol = "BTC-USDT";

    Order order2{ "order-002", "user-B", 100.0, 5.0, false }; // sell
    order2.symbol = "BTC-USDT";

    auto trades = matcher.submit(order1);
    publisher.publish(trades);

    trades = matcher.submit(order2);
    publisher.publish(trades);

    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 0;
}
