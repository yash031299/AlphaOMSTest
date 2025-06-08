#include "grpc_client.hpp"
#include "order.pb.h"
#include "order.grpc.pb.h"
#include <grpcpp/grpcpp.h>
#include <spdlog/spdlog.h>

ExecutionGRPCClient::ExecutionGRPCClient(const std::string& target) {
    // Initialize channel, stub, etc.
    channel = grpc::CreateChannel(target, grpc::InsecureChannelCredentials());
    stub = OrderExecution::NewStub(channel);
}

bool ExecutionGRPCClient::sendOrder(const std::string& json) {
    grpc::ClientContext context;
    OrderRequest req;
    OrderResponse res;

    req.set_json(json);

    grpc::Status status = stub->ExecuteOrder(&context, req, &res);
    return status.ok() && res.success();
}