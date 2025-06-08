#include "funding_scheduler.hpp"
#include <chrono>

FundingScheduler::FundingScheduler(boost::asio::io_context& io,
                                   std::shared_ptr<FundingCalculator> calculator,
                                   std::function<void(const std::string&, const FundingRate&)> callback,
                                   int intervalSeconds)
    : io_(io), calculator_(calculator), callback_(callback), interval_(intervalSeconds) {}

void FundingScheduler::addSymbol(const std::string& symbol) {
    timers_[symbol] = std::make_shared<boost::asio::steady_timer>(io_);
}

void FundingScheduler::start() {
    for (auto& [symbol, timer] : timers_) {
        scheduleNext(symbol);
    }
}

void FundingScheduler::scheduleNext(const std::string& symbol) {
    auto timer = timers_[symbol];
    timer->expires_after(std::chrono::seconds(interval_));
    timer->async_wait([this, symbol](const boost::system::error_code& ec) {
        if (ec) return;

        FundingRate rate = calculator_->computeFunding(symbol);
        callback_(symbol, rate);

        scheduleNext(symbol);  // reschedule
    });
}
