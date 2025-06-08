#include "controller.hpp"

OrderController::OrderController(std::shared_ptr<Validator> validator,
                                 std::shared_ptr<ExecutionClient> exec_client)
    : validator_(std::move(validator)), exec_client_(std::move(exec_client)) {}


grpc::Status OrderController::RouteOrder(grpc::ServerContext*,
                                         const order::OrderRequest* request,
                                         order::OrderResponse* response) {
    if (!validator_->validateOrder(*request)) {
        response->set_status("REJECTED");
        response->set_message("Validation failed");
        return grpc::Status::OK;
    }

    auto result = exec_client_->submitOrder(*request);
    *response = result;
    return grpc::Status::OK;
}
