#pragma once
#include <memory>
#include <grpcpp/grpcpp.h>
#include "pnl.grpc.pb.h"

class PnLClient {
public:
    explicit PnLClient(std::shared_ptr<grpc::Channel> channel);
    bool sendTradeUpdate(const std::string& user_id, const std::string& symbol,
                     double price, double qty, const std::string& side);

private:
    std::unique_ptr<pnl::PnLService::Stub> stub_;
};
