#include "redis_subscriber.hpp"
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
    RedisSubscriber sub;
    sub.subscribeToStream("market_ticks");
    return 0;
}
