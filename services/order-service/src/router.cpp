#include "router.hpp"
#include "validator.hpp"
#include "grpc_client.hpp"
#include <spdlog/spdlog.h>

OrderRouter::OrderRouter() {}

router::OrderResponse OrderRouter::route(const router::OrderRequest& request) {
    router::OrderResponse res;

    if (!Validator::validate(request)) {
        res.set_accepted(false);
        res.set_message("Validation failed.");
        return res;
    }

    bool success = ExecutionClient::sendToExecution(request);
    res.set_accepted(success);
    res.set_message(success ? "Order routed to execution." : "Failed to send to execution.");
    return res;
}
