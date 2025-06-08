#ifndef MARGIN_LISTENER_HPP
#define MARGIN_LISTENER_HPP

#include "redis_safe_client.hpp"
#include "user_wallet_store.hpp"
#include <string>
#include <thread>
#include <atomic>

class MarginEventListener {
public:
    MarginEventListener(std::shared_ptr<RedisSafeClient> redis,
                        std::shared_ptr<UserWalletStore> walletStore);

    void start();
    void stop();

private:
    std::shared_ptr<RedisSafeClient> redis_;
    std::shared_ptr<UserWalletStore> walletStore_;
    std::thread thread_;
    std::atomic<bool> running_{false};
};

#endif // MARGIN_LISTENER_HPP
