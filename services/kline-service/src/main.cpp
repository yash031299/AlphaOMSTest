#include "candle_aggregator.hpp"
#include "kline_scheduler.hpp"
#include <boost/asio.hpp>
#include <spdlog/spdlog.h>
#include <thread>
#include <chrono>
#include <random>

int main() {
    spdlog::set_level(spdlog::level::info);
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%^%l%$] %v");

    SPDLOG_INFO("🚀 Kline Service Starting...");

    boost::asio::io_context io;
    auto aggregator = std::make_shared<CandleAggregator>();

    KlineScheduler scheduler(io, aggregator, [](const std::vector<Candle>& candles) {
        for (const auto& c : candles) {
            SPDLOG_INFO("[{} - {}] O:{:.2f} H:{:.2f} L:{:.2f} C:{:.2f} V:{:.2f} T:{}",
                        c.symbol, c.interval, c.open, c.high, c.low, c.close, c.volume, c.tradeCount);
        }
    }, 10);  // flush every 10 seconds for testing

    scheduler.start();

    std::thread feedThread([aggregator]() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> priceGen(27000.0, 27500.0);
        std::uniform_real_distribution<> qtyGen(0.01, 1.5);

        while (true) {
            TradeTick tick{
                .symbol = "BTC-USDT",
                .price = priceGen(gen),
                .quantity = qtyGen(gen),
                .timestamp = [] {
                    auto now = std::chrono::system_clock::now();
                    std::time_t t = std::chrono::system_clock::to_time_t(now);
                    std::stringstream ss;
                    ss << std::put_time(std::localtime(&t), "%F %T");
                    return ss.str();
                }()
            };
            aggregator->addTrade(tick);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    });

    io.run();
    feedThread.join();
    return 0;
}
