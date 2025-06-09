#include "grpc_server.hpp"
#include "config_loader.hpp"
#include <spdlog/spdlog.h>

int main() {
    SPDLOG_INFO("🛠️ Starting config-service...");

    if (!ConfigLoader::get().loadFile("config.env")) {
        SPDLOG_ERROR("❌ Failed to load config.env");
        return 1;
    }

    ConfigGRPCServer server;
    server.run();
    return 0;
}
