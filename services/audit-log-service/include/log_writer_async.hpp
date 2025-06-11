#ifndef ASYNC_LOG_WRITER_HPP
#define ASYNC_LOG_WRITER_HPP

#include "log_writer.hpp"
#include "log_event.pb.h"
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include <variant>
#include <atomic>
#include <chrono>


using LogEventVariant = std::variant<TradeEvent, OrderEvent, FundingEvent, LiquidationEvent>;

class AsyncLogWriter {
public:
    static AsyncLogWriter& get();

    void enqueue(LogEventVariant event);
    void stop();

private:
    AsyncLogWriter();
    ~AsyncLogWriter();

    void workerThread();

    std::thread thread_;
    std::queue<LogEventVariant> queue_;
    std::mutex mutex_;
    std::condition_variable cv_;
    bool running_;
    LogWriter writer_;
    std::atomic<int> write_count_;
    std::chrono::steady_clock::time_point last_metrics_dump_;

};

#endif // ASYNC_LOG_WRITER_HPP
