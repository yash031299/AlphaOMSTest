#ifndef LOG_WRITER_HPP
#define LOG_WRITER_HPP

#include "log_event.pb.h"
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

    std::string todayDate();

    // For unit tests
    std::string toJson(const TradeEvent& event);
    std::string toJson(const OrderEvent& event);
    std::string toJson(const FundingEvent& event);
    std::string toJson(const LiquidationEvent& event);

private:
    std::string baseDir_;
    std::mutex mutex_;

    void appendToFile(const std::string& path, const std::string& json);
};

#endif // LOG_WRITER_HPP
