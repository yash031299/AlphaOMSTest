#pragma once

#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "order.grpc.pb.h"

class ExecutionClient {
public:
    explicit ExecutionClient(std::shared_ptr<grpc::Channel> channel);

    order::OrderResponse submitOrder(const order::OrderRequest& request);

private:
    std::unique_ptr<order::OrderService::Stub> stub_;
};
