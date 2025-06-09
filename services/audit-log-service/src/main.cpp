#include "grpc_server.hpp"
#include <spdlog/spdlog.h>

int main() {
    SPDLOG_INFO("🔐 Starting audit-log-service (gRPC mode)...");
    GRPCServer server;
    server.run();  // Will block and serve indefinitely
    return 0;
}
