#include "scheduler.hpp"
#include <spdlog/spdlog.h>

using namespace std::chrono_literals;

WalletFlushScheduler::WalletFlushScheduler(std::shared_ptr<UserWalletStore> walletStore)
    : walletStore_(std::move(walletStore)) {}

void WalletFlushScheduler::start() {
    running_ = true;
    thread_ = std::thread([this]() {
        while (running_) {
            std::this_thread::sleep_for(5s);  // flush every 5 seconds
            walletStore_->flushToRedis();
        }
    });
    SPDLOG_INFO("Wallet flush scheduler started");
}

void WalletFlushScheduler::stop() {
    running_ = false;
    if (thread_.joinable()) thread_.join();
    SPDLOG_INFO("Wallet flush scheduler stopped");
}
