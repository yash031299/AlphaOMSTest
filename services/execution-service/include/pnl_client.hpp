#pragma once
#include <memory>
#include <grpcpp/grpcpp.h>
#include "pnl.grpc.pb.h"

class PnLClient {
public:
    explicit PnLClient(std::shared_ptr<grpc::Channel> channel);
    void sendTradeUpdate(const std::string& user_id, const std::string& symbol,
                         double price, double quantity, const std::string& side);

private:
    std::unique_ptr<pnl::PnLService::Stub> stub_;
};
