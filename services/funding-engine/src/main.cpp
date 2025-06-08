#include "funding_calculator.hpp"
#include "funding_scheduler.hpp"
#include "funding_publisher.hpp"

#include <boost/asio.hpp>
#include <spdlog/spdlog.h>
#include <thread>

int main() {
    auto logger = spdlog::stdout_color_mt("funding-engine");
    spdlog::set_level(spdlog::level::info);
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%^%l%$] %v");

    SPDLOG_INFO("🚀 Starting Funding Engine...");

    boost::asio::io_context io;
    auto calculator = std::make_shared<FundingCalculator>(0.01);
    FundingPublisher publisher;

    FundingScheduler scheduler(io, calculator,
        [&](const std::string& symbol, const FundingRate& rate) {
            publisher.publish(symbol, rate);
        }, 5  // every 5 seconds for test
    );

    scheduler.addSymbol("BTC-USDT");
    scheduler.addSymbol("ETH-USDT");

    calculator->updateOpenInterest("BTC-USDT", 5500.0, 4500.0);
    calculator->updateOpenInterest("ETH-USDT", 1200.0, 800.0);

    scheduler.start();

    std::thread([&io] {
        io.run();
    }).join();

    return 0;
}
