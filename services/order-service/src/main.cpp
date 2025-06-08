#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "controller.hpp"
#include "symbol_client.hpp"
#include "user_client.hpp"
#include "validator.hpp"
#include "grpc_client.hpp"

void setupLogging() {
    auto logger = spdlog::stdout_color_mt("console");
    spdlog::set_default_logger(logger);
    spdlog::set_level(spdlog::level::info);
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%^%l%$] %v");
}

int main() {
    setupLogging();

    std::string server_address("0.0.0.0:50052");

    // Create gRPC channels to other services
    auto symbolChannel = grpc::CreateChannel("symbol-registry-service:50051", grpc::InsecureChannelCredentials());
    auto userChannel   = grpc::CreateChannel("user-service:50053", grpc::InsecureChannelCredentials());
    auto execChannel   = grpc::CreateChannel("execution-service:50054", grpc::InsecureChannelCredentials());

    // Instantiate gRPC clients
    auto symbolClient = std::make_shared<SymbolClient>(symbolChannel);
    auto userClient   = std::make_shared<UserClient>(userChannel);
    auto execClient   = std::make_shared<ExecutionClient>(execChannel);

    // Create validator and inject clients
    auto validator = std::make_shared<Validator>(symbolClient, userClient);

    // Create and wire OrderController
    auto controller = std::make_unique<OrderController>(validator, execClient);

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(controller.get());

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    SPDLOG_INFO("OrderService listening on {}", server_address);
    server->Wait();

    return 0;
}
