#pragma once
#include <string>
#include <memory>
#include <sw/redis++/redis++.h>
#include "order.pb.h"

class RedisPublisher {
public:
    RedisPublisher();

    void publishTrade(const order::OrderRequest& req, const std::string& trade_id);
    void publishReject(const order::OrderRequest& req, const std::string& reason);

private:
    std::shared_ptr<sw::redis::Redis> redis_;
};
