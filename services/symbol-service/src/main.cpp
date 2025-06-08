#include "orderbook_manager.hpp"
#include "snapshot_generator.hpp"
#include <spdlog/spdlog.h>
#include <thread>
#include <chrono>

extern void runHealthMonitor(const std::string& serviceName, int intervalSeconds);

int main() {
    // Logger setup
    auto logger = spdlog::stdout_color_mt("symbol-service");
    spdlog::set_default_logger(logger);
    spdlog::set_level(spdlog::level::info);
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%^%l%$] %v");

    SPDLOG_INFO("🚀 Starting symbol-service...");

    OrderBookManager book;
    std::string symbol = "BTC-USDT";

    // Insert mock orders for testing
    book.addOrder(symbol, Order{"buy1", 25980.0, 0.75, true});
    book.addOrder(symbol, Order{"sell1", 25990.0, 1.20, false});

    // Start health monitor
    runHealthMonitor("symbol-service");

    // Periodically publish L1 snapshot
    while (true) {
        auto top = book.getTopOfBook(symbol, 1);
        std::string snapshot = SnapshotGenerator::generateJsonL1(symbol, top);
        SPDLOG_INFO("[SNAPSHOT] {}", snapshot);

        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    return 0;
}
