#ifndef KLINE_SCHEDULER_HPP
#define KLINE_SCHEDULER_HPP

#include "candle_aggregator.hpp"
#include <boost/asio.hpp>
#include <functional>
#include <memory>

class KlineScheduler {
public:
    KlineScheduler(boost::asio::io_context& io,
                   std::shared_ptr<CandleAggregator> aggregator,
                   std::function<void(const std::vector<Candle>&)> callback,
                   int intervalSeconds = 60);

    void start();

private:
    void scheduleNext();

    boost::asio::io_context& io_;
    std::shared_ptr<CandleAggregator> aggregator_;
    std::function<void(const std::vector<Candle>&)> callback_;
    int interval_;
    std::shared_ptr<boost::asio::steady_timer> timer_;
};

#endif // KLINE_SCHEDULER_HPP
