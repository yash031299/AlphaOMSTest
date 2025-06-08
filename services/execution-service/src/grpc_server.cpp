#include "grpc_server.hpp"
#include "execution_router.hpp"
#include <spdlog/spdlog.h>

grpc::Status ExecutionServiceImpl::SubmitOrder(grpc::ServerContext*,
                                               const order::OrderRequest* request,
                                               order::OrderResponse* response) {
    SPDLOG_INFO("ExecutionService received order for symbol={}, user={}", request->symbol(), request->user_id());
    *response = ExecutionRouter::route(*request);
    return grpc::Status::OK;
}
