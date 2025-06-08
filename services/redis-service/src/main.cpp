#include "order_updater.hpp"
#include "trade_broadcaster.hpp"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

void setupLogging() {
    auto logger = spdlog::stdout_color_mt("console");
    spdlog::set_default_logger(logger);
    spdlog::set_level(spdlog::level::info);
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%l] %v");
}

int main() {
    setupLogging();
    OrderUpdater updater;
    updater.updateStatus("abc123", "FILLED");

    TradeBroadcaster broadcaster;
    broadcaster.publishTrade(R"({"orderId":"abc123", "price": 25000, "qty": 0.5})");

    std::thread healthThread([] {
        startHealthCheckServer();
    });
    healthThread.detach();

    return 0;
}
