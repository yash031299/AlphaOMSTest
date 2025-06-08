#include "anomaly_detector.hpp"
#include <chrono>
#include <ctime>
#include <sstream>
#include <spdlog/spdlog.h>

AnomalyDetector::AnomalyDetector(boost::asio::io_context& io,
                                 std::shared_ptr<RuleEngine> ruleEngine,
                                 std::shared_ptr<AlertDispatcher> dispatcher)
    : io_(io), ruleEngine_(ruleEngine), dispatcher_(dispatcher) {
    timer_ = std::make_shared<boost::asio::steady_timer>(io_);
}

void AnomalyDetector::start() {
    poll();
}

void AnomalyDetector::poll() {
    using namespace std::chrono_literals;

    // Simulated user data
    std::vector<AlertContext> users = {
        { "user-001", "BTC-USDT", 92.5, 110.0, 60.1, 25.0, "" },
        { "user-002", "ETH-USDT", 5.8, 9.0, -150.0, 3.0, "" },
        { "user-003", "BTC-USDT", 22.0, 35.0, 4.2, 8.0, "" }
    };

    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::string timestamp = std::ctime(&t);
    timestamp.pop_back();

    for (auto& ctx : users) {
        ctx.timestamp = timestamp;
        auto alerts = ruleEngine_->evaluate(ctx);
        for (const auto& alert : alerts) {
            dispatcher_->dispatch(ctx.userId, alert);
        }
    }

    timer_->expires_after(10s);
    timer_->async_wait([this](auto) { poll(); });
}
