#pragma once
#include <thread>
#include <atomic>

class RedisAnomalyListener {
public:
    void start();
    void stop();

private:
    void listen();
    std::thread listenerThread;
    std::atomic<bool> running{false};
};
