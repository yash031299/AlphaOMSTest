#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "grpc_interface.hpp"

int main() {
    auto logger = spdlog::stdout_color_mt("pnl-service");
    spdlog::set_default_logger(logger);
    spdlog::set_level(spdlog::level::info);
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%^%l%$] %v");

    SPDLOG_INFO("🚀 Starting PnL Service...");

    std::string server_address = "0.0.0.0:7000";
    PnLServiceImpl service;

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    SPDLOG_INFO("📡 gRPC server listening on {}", server_address);

    server->Wait();
    return 0;
}
