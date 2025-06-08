#ifndef FUNDING_SCHEDULER_HPP
#define FUNDING_SCHEDULER_HPP

#include "funding_calculator.hpp"
#include <boost/asio.hpp>
#include <functional>
#include <memory>
#include <string>

class FundingScheduler {
public:
    FundingScheduler(boost::asio::io_context& io,
                     std::shared_ptr<FundingCalculator> calculator,
                     std::function<void(const std::string&, const FundingRate&)> callback,
                     int intervalSeconds = 10);

    void addSymbol(const std::string& symbol);
    void start();

private:
    void scheduleNext(const std::string& symbol);

    boost::asio::io_context& io_;
    std::shared_ptr<FundingCalculator> calculator_;
    std::function<void(const std::string&, const FundingRate&)> callback_;
    int interval_;
    std::unordered_map<std::string, std::shared_ptr<boost::asio::steady_timer>> timers_;
};

#endif // FUNDING_SCHEDULER_HPP
