#include "user_metadata_store.hpp"
#include <spdlog/spdlog.h>

UserMetadataStore::UserMetadataStore(std::shared_ptr<RedisSafeClient> redis)
    : redis_(std::move(redis)) {}

void UserMetadataStore::initializeFromRedis() {
    redisReply* reply = redis_->command("HGETALL user_metadata");
    if (!reply || reply->type != REDIS_REPLY_ARRAY) {
        SPDLOG_WARN("Could not fetch user_metadata from Redis");
        if (reply) freeReplyObject(reply);
        return;
    }

    std::lock_guard<std::mutex> lock(mutex_);

    for (size_t i = 0; i < reply->elements; i += 2) {
        std::string userId = reply->element[i]->str;
        std::string rawJson = reply->element[i + 1]->str;

        // Simulate parsed metadata
        UserMetadata meta;
        if (rawJson.find("kyc:true") != std::string::npos) meta.kycVerified = true;
        if (rawJson.find("risk:high") != std::string::npos) meta.riskTier = "high";
        if (rawJson.find("lev:50") != std::string::npos) meta.maxLeverage = 50.0;

        metadata_[userId] = meta;
    }

    SPDLOG_INFO("Loaded metadata for {} users", metadata_.size());
    freeReplyObject(reply);
}

UserMetadata UserMetadataStore::getMetadata(const std::string& userId) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = metadata_.find(userId);
    return it != metadata_.end() ? it->second : UserMetadata{};
}
