#include "redis_safe_client.hpp"
#include "user_wallet_store.hpp"
#include "user_metadata_store.hpp"
#include "margin_listener.hpp"
#include "scheduler.hpp"
#include <spdlog/spdlog.h>
#include <memory>

extern void runHealthCheck(const std::string& serviceName, int intervalSeconds);

int main() {
    // Set up logger
    auto logger = spdlog::stdout_color_mt("user-service");
    spdlog::set_default_logger(logger);
    spdlog::set_level(spdlog::level::info);
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%^%l%$] %v");

    SPDLOG_INFO("🚀 Starting user-service...");

    // Redis connection
    std::string redisHost = std::getenv("REDIS_HOST") ? std::getenv("REDIS_HOST") : "127.0.0.1";
    int redisPort = std::getenv("REDIS_PORT") ? std::stoi(std::getenv("REDIS_PORT")) : 6379;
    auto redis = std::make_shared<RedisSafeClient>(redisHost, redisPort);

    // Wallet & metadata stores
    auto walletStore   = std::make_shared<UserWalletStore>(redis);
    auto metadataStore = std::make_shared<UserMetadataStore>(redis);

    walletStore->initializeFromRedis();
    metadataStore->initializeFromRedis();

    // Margin update listener
    MarginEventListener listener(redis, walletStore);
    listener.start();

    // Periodic Redis flush
    WalletFlushScheduler scheduler(walletStore);
    scheduler.start();

    // Health log
    runHealthCheck("user-service", 15);

    // Keep main thread alive
    SPDLOG_INFO("user-service is up and running.");
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(60));
    }

    return 0;
}
