#pragma once
#include <thread>
#include <atomic>
#include "ledger.hpp"

class MetricsServer {
public:
    MetricsServer(std::shared_ptr<Ledger> ledger, int port = 8081);
    void start();
    void stop();

private:
    std::thread server_thread_;
    std::atomic<bool> running_;
    std::shared_ptr<Ledger> ledger_;
    int port_;
    std::atomic<int> snapshot_count = 0;
};
