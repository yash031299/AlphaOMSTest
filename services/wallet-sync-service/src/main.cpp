#include "redis_listener.hpp"
#include <spdlog/spdlog.h>
#include <thread>
int startHealthCheckServer();  // Declare it

int main() {
    spdlog::info("[wallet-sync-service] Starting...");

    // Start Redis pub/sub listener on background thread
    std::thread redisThread([] {
        RedisTradeListener listener;
        listener.start("127.0.0.1", 6379);
    });

    redisThread.detach();
    spdlog::info("[wallet-sync-service] Redis listener thread started");

    // Start HTTP health server
    std::thread healthThread([] {
        startHealthCheckServer();
    });
    healthThread.detach();

    // Main thread sleeps forever
    while (true) std::this_thread::sleep_for(std::chrono::hours(24));
    return 0;
}
