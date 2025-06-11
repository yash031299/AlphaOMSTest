#include "log_writer_async.hpp"
#include <spdlog/spdlog.h>

AsyncLogWriter::AsyncLogWriter()
    : running_(true), writer_("data/logs") {
    thread_ = std::thread(&AsyncLogWriter::workerThread, this);
    write_count_ = 0;
    last_metrics_dump_ = std::chrono::steady_clock::now();

}

AsyncLogWriter::~AsyncLogWriter() {
    stop();
}

AsyncLogWriter& AsyncLogWriter::get() {
    static AsyncLogWriter instance;
    return instance;
}

void AsyncLogWriter::stop() {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        running_ = false;
    }
    cv_.notify_all();
    if (thread_.joinable()) {
        thread_.join();
    }
}

void AsyncLogWriter::enqueue(LogEventVariant event) {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(event);
    }
    cv_.notify_one();
}

void AsyncLogWriter::workerThread() {
    SPDLOG_INFO("AsyncLogWriter thread started");

    while (true) {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [this] { return !queue_.empty() || !running_; });

        if (!running_ && queue_.empty())
            break;  // safe shutdown

        while (!queue_.empty()) {
            auto event = queue_.front();
            queue_.pop();
            lock.unlock();

            try {
                std::visit([this](auto&& e) {
                    using T = std::decay_t<decltype(e)>;
                    if constexpr (std::is_same_v<T, TradeEvent>)
                        writer_.writeTrade(e);
                    else if constexpr (std::is_same_v<T, OrderEvent>)
                        writer_.writeOrder(e);
                    else if constexpr (std::is_same_v<T, FundingEvent>)
                        writer_.writeFunding(e);
                    else if constexpr (std::is_same_v<T, LiquidationEvent>)
                        writer_.writeLiquidation(e);
                }, event);

                ++write_count_;
                auto now = std::chrono::steady_clock::now();
                double elapsed_sec = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_metrics_dump_).count() / 1000.0;
                if (elapsed_sec >= 5.0) {
                    SPDLOG_INFO("📊 Log write rate: {:.2f} events/sec", write_count_ / elapsed_sec);
                    write_count_ = 0;
                    last_metrics_dump_ = now;
                }

            } catch (const std::exception& e) {
                SPDLOG_ERROR("Async writer error: {}", e.what());
            }

            lock.lock();  // reacquire to safely pop next
        }
    }

    SPDLOG_INFO("AsyncLogWriter thread exiting");
}
