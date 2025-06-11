#include "user_client.hpp"
#include <spdlog/spdlog.h>
#include <cstdlib>
#include <thread>
#include <chrono>
#include "user.pb.h"
#include "user.grpc.pb.h"


UserWalletClient::UserWalletClient(std::shared_ptr<grpc::Channel> channel)
    : stub_(user::UserService::NewStub(channel)) {}

bool UserWalletClient::updateWallet(const std::string& user_id,
                                    double amount,
                                    const std::string& reason) {
    user::WalletUpdateRequest request;
    request.set_user_id(user_id);
    request.set_amount_change(amount);
    request.set_reason(reason);

    grpc::Status status;

    for (int attempt = 1; attempt <= 3; ++attempt) {
        grpc::ClientContext context;
        user::WalletResponse response;

        try {
            status = stub_->UpdateWallet(&context, request, &response);
        } catch (const std::exception& e) {
            SPDLOG_ERROR("❌ gRPC exception (UserWallet) on attempt {}: {}", attempt, e.what());
        } catch (...) {
            SPDLOG_ERROR("❌ Unknown exception (UserWallet) on attempt {}", attempt);
        }

        if (status.ok()) {
            SPDLOG_INFO("✅ Wallet update succeeded on attempt {} for user: {}", attempt, user_id);
            return true;
        }

        SPDLOG_WARN("⚠️ Wallet update failed (attempt {}): {}", attempt, status.error_message());
        std::this_thread::sleep_for(std::chrono::milliseconds(100 * (1 << (attempt - 1))));
    }

    SPDLOG_ERROR("❌ Wallet update failed after all retries for user: {}", user_id);
    return false;
}
