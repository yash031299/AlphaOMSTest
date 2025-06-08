#include <grpcpp/server.h>
#include "grpc_server.cpp"
#include <grpcpp/server_builder.h>
#include "order.grpc.pb.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>


void setupLogging() {
    auto logger = spdlog::stdout_color_mt("console");
    spdlog::set_default_logger(logger);
    spdlog::set_level(spdlog::level::info);
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%l] %v");
}

int main() {
    setupLogging();
    std::string server_address("0.0.0.0:50051");
    OrderExecutionServiceImpl service;

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());

    SPDLOG_INFO("gRPC Server listening on {}", server_address);
    server->Wait();
    return 0;
}
