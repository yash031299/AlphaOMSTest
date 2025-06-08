#ifndef USER_METADATA_STORE_HPP
#define USER_METADATA_STORE_HPP

#include <string>
#include <unordered_map>
#include <mutex>
#include <memory>
#include "redis_safe_client.hpp"

struct UserMetadata {
    bool kycVerified = false;
    std::string riskTier = "normal";   // normal, high
    double maxLeverage = 10.0;
};

class UserMetadataStore {
public:
    explicit UserMetadataStore(std::shared_ptr<RedisSafeClient> redis);

    void initializeFromRedis();                             // Load all user metadata
    UserMetadata getMetadata(const std::string& userId);    // Get metadata for user

private:
    std::shared_ptr<RedisSafeClient> redis_;
    std::unordered_map<std::string, UserMetadata> metadata_;
    std::mutex mutex_;
};

#endif // USER_METADATA_STORE_HPP
