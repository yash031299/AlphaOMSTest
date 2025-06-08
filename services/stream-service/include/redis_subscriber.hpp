#pragma once
#include <string>

class RedisSubscriber {
public:
    void subscribeToStream(const std::string& streamKey);
};
