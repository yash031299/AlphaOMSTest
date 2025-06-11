#pragma once

#include <memory>
#include <grpcpp/grpcpp.h>
#include "user.grpc.pb.h"

class UserWalletClient {
public:
    explicit UserWalletClient(std::shared_ptr<grpc::Channel> channel);

    bool updateWallet(const std::string& user_id,
                      double amount_change,
                      const std::string& reason);
private:
    std::unique_ptr<user::UserService::Stub> stub_;
};
