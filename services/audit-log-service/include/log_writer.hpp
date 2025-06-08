#ifndef LOG_WRITER_HPP
#define LOG_WRITER_HPP

#include "event_types.hpp"
#include <fstream>
#include <mutex>
#include <string>

class LogWriter {
public:
    explicit LogWriter(const std::string& baseDir);

    void writeTrade(const TradeEvent& event);
    void writeOrder(const OrderEvent& event);
    void writeFunding(const FundingEvent& event);
    void writeLiquidation(const LiquidationEvent& event);

private:
    std::string baseDir_;
    std::mutex mutex_;

    std::string todayDate();
    std::string toJson(const TradeEvent& event);
    std::string toJson(const OrderEvent& event);
    std::string toJson(const FundingEvent& event);
    std::string toJson(const LiquidationEvent& event);

    void appendToFile(const std::string& path, const std::string& json);
};

#endif // LOG_WRITER_HPP
