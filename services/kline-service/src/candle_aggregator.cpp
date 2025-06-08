#include "candle_aggregator.hpp"
#include <chrono>
#include <sstream>
#include <iomanip>

CandleAggregator::CandleAggregator() {}

std::string CandleAggregator::getStartTime(const std::string& iso, const std::string& interval) {
    std::tm t{};
    std::istringstream ss(iso);
    ss >> std::get_time(&t, "%Y-%m-%d %H:%M:%S");

    int period = 60; // default 1m
    if (interval == "5m") period = 300;
    else if (interval == "1h") period = 3600;

    std::time_t ts = std::mktime(&t);
    ts = ts - (ts % period); // round down

    std::ostringstream rounded;
    rounded << std::put_time(std::localtime(&ts), "%F %T");
    return rounded.str(); // e.g., "2025-06-07 22:05:00"
}

std::string CandleAggregator::makeKey(const std::string& symbol, const std::string& interval, const std::string& start) {
    return symbol + ":" + interval + ":" + start;
}

void CandleAggregator::addTrade(const TradeTick& tick) {
    std::lock_guard<std::mutex> lock(mutex_);

    for (const std::string& interval : {"1m", "5m", "1h"}) {
        std::string start = getStartTime(tick.timestamp, interval);
        std::string key = makeKey(tick.symbol, interval, start);

        Candle& c = candleMap_[key];
        if (c.tradeCount == 0) {
            c.symbol = tick.symbol;
            c.interval = interval;
            c.startTime = start;
            c.open = c.high = c.low = c.close = tick.price;
            c.volume = tick.quantity;
            c.tradeCount = 1;
        } else {
            c.high = std::max(c.high, tick.price);
            c.low = std::min(c.low, tick.price);
            c.close = tick.price;
            c.volume += tick.quantity;
            c.tradeCount++;
        }
    }
}

std::vector<Candle> CandleAggregator::flushCandles() {
    std::lock_guard<std::mutex> lock(mutex_);
    std::vector<Candle> flushed;

    for (auto& [_, candle] : candleMap_) {
        flushed.push_back(candle);
    }

    candleMap_.clear();
    return flushed;
}
