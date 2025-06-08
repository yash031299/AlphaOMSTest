#ifndef CANDLE_AGGREGATOR_HPP
#define CANDLE_AGGREGATOR_HPP

#include "candle_types.hpp"
#include <unordered_map>
#include <map>
#include <string>
#include <mutex>

class CandleAggregator {
public:
    CandleAggregator();

    void addTrade(const TradeTick& tick);
    std::vector<Candle> flushCandles();

private:
    std::mutex mutex_;

    // Key = symbol + interval + startTime
    std::map<std::string, Candle> candleMap_;

    std::string getStartTime(const std::string& isoTimestamp, const std::string& interval);
    std::string makeKey(const std::string& symbol, const std::string& interval, const std::string& startTime);
};

#endif // CANDLE_AGGREGATOR_HPP
