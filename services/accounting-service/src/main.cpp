#include "ledger.hpp"
#include "funding_handler.hpp"
#include "pnl_tracker.hpp"
#include "settlement_engine.hpp"

#include <boost/asio.hpp>
#include <spdlog/spdlog.h>
#include <thread>
#include <chrono>

int main() {
    spdlog::set_level(spdlog::level::info);
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%^%l%$] %v");

    SPDLOG_INFO("🚀 Accounting Service Starting...");

    boost::asio::io_context io;
    auto ledger = std::make_shared<Ledger>();

    // Hook up subsystems
    FundingHandler funding(ledger);
    PnLTracker pnl(ledger);
    SettlementEngine settlement(io, ledger, 30);  // for testing: 30s snapshot

    settlement.start();

    // Simulate events
    std::thread eventFeeder([&]() {
        using namespace std::chrono_literals;

        while (true) {
            std::string now = [] {
                auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                std::stringstream ss;
                ss << std::put_time(std::localtime(&t), "%F %T");
                return ss.str();
            }();

            pnl.recordRealizedPnL("user-001", "BTC-USDT", 45.5, now, "close-ord-001");
            funding.applyFunding("user-001", "BTC-USDT", -2.3, now, "fund-cycle-10");

            std::this_thread::sleep_for(10s);
        }
    });

    io.run();
    eventFeeder.join();
    return 0;
}
