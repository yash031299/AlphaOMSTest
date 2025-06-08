#include "settlement_engine.hpp"
#include <spdlog/spdlog.h>
#include <chrono>
#include <fstream>
#include <sstream>
#include <iomanip>

SettlementEngine::SettlementEngine(boost::asio::io_context& io,
                                   std::shared_ptr<Ledger> ledger,
                                   int intervalSeconds)
    : io_(io), ledger_(ledger), interval_(intervalSeconds) {
    timer_ = std::make_shared<boost::asio::steady_timer>(io_);
}

void SettlementEngine::start() {
    scheduleNext();
}

void SettlementEngine::scheduleNext() {
    timer_->expires_after(std::chrono::seconds(interval_));
    timer_->async_wait([this](const boost::system::error_code& ec) {
        if (!ec) {
            performSettlement();
            scheduleNext();
        }
    });
}

void SettlementEngine::performSettlement() {
    SPDLOG_INFO("⏳ Performing PnL settlement and ledger snapshot...");

    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::stringstream filename;
    filename << "data/snapshots/ledger_" << std::put_time(std::localtime(&t), "%F") << ".csv";

    std::ofstream out(filename.str());
    out << "userId,symbol,amount,type,reference,timestamp\n";

    for (const auto& userId : {"user-001", "user-002", "user-003"}) {
        auto entries = ledger_->getEntries(userId);
        for (const auto& e : entries) {
            out << e.userId << "," << e.symbol << "," << e.amount << "," << e.type << ","
                << e.reference << "," << e.timestamp << "\n";
        }
    }

    SPDLOG_INFO("✅ Ledger snapshot written to {}", filename.str());
}
