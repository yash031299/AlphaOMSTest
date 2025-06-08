#include <thread>
#include <chrono>
#include <spdlog/spdlog.h>

void runHealthCheck(const std::string& serviceName, int intervalSeconds = 10) {
    std::thread([serviceName, intervalSeconds]() {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(intervalSeconds));
            SPDLOG_INFO("[HEALTH] {} is alive and running", serviceName);
        }
    }).detach();
}
