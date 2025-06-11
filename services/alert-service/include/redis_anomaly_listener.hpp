#pragma once
#include "alert_dispatcher.hpp"
#include <thread>
#include <atomic>
#include <memory>

class RedisAnomalyListener {
public:
    RedisAnomalyListener(std::shared_ptr<AlertDispatcher> dispatcher);
    void start();
    void stop();

private:
    void listen();
    std::shared_ptr<AlertDispatcher> dispatcher_;
    std::thread listenerThread;
    std::atomic<bool> running{false};
};
