#pragma once
#include "order.grpc.pb.h"

class ExecutionServiceImpl final : public order::OrderService::Service {
public:
    grpc::Status SubmitOrder(grpc::ServerContext*,
                             const order::OrderRequest*,
                             order::OrderResponse*) override;
};
