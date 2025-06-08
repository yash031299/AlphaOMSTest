#ifndef USER_WALLET_STORE_HPP
#define USER_WALLET_STORE_HPP

#include "redis_safe_client.hpp"
#include <unordered_map>
#include <string>
#include <mutex>

class UserWalletStore {
public:
    explicit UserWalletStore(std::shared_ptr<RedisSafeClient> redis);

    void initializeFromRedis();                      // Load all user balances from Redis
    void applyDelta(const std::string& userId, double delta);  // Apply margin change
    void flushToRedis();                             // Flush in-memory state back to Redis

    double getBalance(const std::string& userId) const;

private:
    std::shared_ptr<RedisSafeClient> redis_;
    mutable std::mutex mutex_;

    std::unordered_map<std::string, double> walletMap_;
};

#endif // USER_WALLET_STORE_HPP
