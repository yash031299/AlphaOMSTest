#include "grpc_interface.hpp"
#include <spdlog/spdlog.h>

int main() {
    SPDLOG_INFO("💰 Starting accounting-service...");
    AccountingGRPCServer server;
    server.run();
    return 0;
}
