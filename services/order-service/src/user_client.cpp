#include "user_client.hpp"
#include <spdlog/spdlog.h>

UserClient::UserClient(std::shared_ptr<grpc::Channel> channel)
    : stub_(user::UserService::NewStub(channel)) {}

std::optional<UserMeta> UserClient::getUserMeta(const std::string& user_id) {
    user::UserIdRequest request;
    request.set_user_id(user_id);
    user::UserMetadataResponse response;
    grpc::ClientContext context;

    auto status = stub_->GetUserMetadata(&context, request, &response);
    if (!status.ok()) {
        SPDLOG_ERROR("GetUserMetadata failed: {}", status.error_message());
        return std::nullopt;
    }

    return UserMeta{
        .user_id = response.user_id(),
        .role = response.role(),
        .kyc_verified = response.kyc_verified()
    };
}

std::optional<WalletInfo> UserClient::getWallet(const std::string& user_id) {
    user::UserWalletRequest request;
    request.set_user_id(user_id);
    user::WalletResponse response;
    grpc::ClientContext context;

    auto status = stub_->GetWallet(&context, request, &response);
    if (!status.ok()) {
        SPDLOG_ERROR("GetWallet failed: {}", status.error_message());
        return std::nullopt;
    }

    return WalletInfo{
        .user_id = response.user_id(),
        .available_balance = response.available_balance(),
        .used_margin = response.used_margin()
    };
}
