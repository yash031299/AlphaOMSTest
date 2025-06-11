#pragma once
#include <string>
#include <memory>
#include <unordered_set>
#include <hiredis/hiredis.h>
#include <thread>
#include <mutex>
#include "ledger.hpp"
#include "funding_handler.hpp"
#include "pnl_tracker.hpp"

class RedisListener {
public:
    RedisListener(const std::string& redis_host,
                  int redis_port,
                  std::shared_ptr<Ledger> ledger);
    ~RedisListener();

    void start();
    void stop();

private:
    void listenLoop();
    void handleMessage(const std::string& json);

    std::shared_ptr<Ledger> ledger_;
    std::unique_ptr<FundingHandler> fundingHandler_;
    std::unique_ptr<PnLTracker> pnlTracker_;

    redisContext* redis_ctx_;
    std::thread listener_thread_;
    std::atomic<bool> running_;
    std::unordered_set<std::string> seen_ids_;
    std::mutex id_mutex_;
};
