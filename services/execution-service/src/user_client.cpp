#include "user_client.hpp"
#include <spdlog/spdlog.h>

UserWalletClient::UserWalletClient(std::shared_ptr<grpc::Channel> channel)
    : stub_(user::UserService::NewStub(channel)) {}

void UserWalletClient::updateWallet(const std::string& user_id,
                                    double amount_change,
                                    const std::string& reason) {
    user::WalletUpdateRequest req;
    req.set_user_id(user_id);
    req.set_amount_change(amount_change);
    req.set_reason(reason);

    user::WalletResponse resp;
    grpc::ClientContext ctx;

    auto status = stub_->UpdateWallet(&ctx, req, &resp);
    if (!status.ok()) {
        SPDLOG_ERROR("Wallet update failed for {}: {}", user_id, status.error_message());
    } else {
        SPDLOG_INFO("Wallet updated: new balance = {}, success = {}", resp.new_balance(), resp.success());
    }
}
