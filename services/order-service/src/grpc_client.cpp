#include "grpc_client.hpp"
#include <spdlog/spdlog.h>

ExecutionClient::ExecutionClient(std::shared_ptr<grpc::Channel> channel)
    : stub_(order::OrderService::NewStub(channel)) {}

order::OrderResponse ExecutionClient::submitOrder(const order::OrderRequest& request) {
    order::OrderResponse response;
    grpc::ClientContext context;

    grpc::Status status = stub_->SubmitOrder(&context, request, &response);

    if (!status.ok()) {
        SPDLOG_ERROR("ExecutionService SubmitOrder failed: {}", status.error_message());
        response.set_status("REJECTED");
        response.set_message("Execution RPC failed");
    }

    return response;
}
