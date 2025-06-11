#ifndef REDIS_CONSUMER_HPP
#define REDIS_CONSUMER_HPP

#include <atomic>
#include <string>
#include <thread>
#include <hiredis/hiredis.h>

class RedisConsumer {
public:
    RedisConsumer(const std::string& host, int port, const std::string& stream);
    ~RedisConsumer();

    void start();
    void stop();

private:
    void run();

    std::string host_;
    int port_;
    std::string stream_;
    std::atomic<bool> running_;
    std::thread worker_;
    redisContext* redis_;
};

#endif // REDIS_CONSUMER_HPP
