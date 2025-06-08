#pragma once

#include <memory>
#include <string>
#include <optional>
#include <grpcpp/grpcpp.h>
#include "user.grpc.pb.h"

struct UserMeta {
    std::string user_id;
    std::string role;
    bool kyc_verified;
};

struct WalletInfo {
    std::string user_id;
    double available_balance;
    double used_margin;
};

class UserClient {
public:
    explicit UserClient(std::shared_ptr<grpc::Channel> channel);

    std::optional<UserMeta> getUserMeta(const std::string& user_id);
    std::optional<WalletInfo> getWallet(const std::string& user_id);

private:
    std::unique_ptr<user::UserService::Stub> stub_;
};
