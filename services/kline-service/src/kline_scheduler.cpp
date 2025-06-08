#include "kline_scheduler.hpp"
#include <chrono>

KlineScheduler::KlineScheduler(boost::asio::io_context& io,
                               std::shared_ptr<CandleAggregator> aggregator,
                               std::function<void(const std::vector<Candle>&)> callback,
                               int intervalSeconds)
    : io_(io), aggregator_(aggregator), callback_(callback), interval_(intervalSeconds) {
    timer_ = std::make_shared<boost::asio::steady_timer>(io_);
}

void KlineScheduler::start() {
    scheduleNext();
}

void KlineScheduler::scheduleNext() {
    timer_->expires_after(std::chrono::seconds(interval_));
    timer_->async_wait([this](const boost::system::error_code& ec) {
        if (ec) return;

        auto candles = aggregator_->flushCandles();
        if (!candles.empty()) {
            callback_(candles);
        }

        scheduleNext();  // loop again
    });
}
