#include "config_loader.hpp"
#include "grpc_server.hpp"
#include <spdlog/spdlog.h>
#include <csignal>
#include <cstdlib>

// 🔁 Signal handler for hot reload
void handleSIGHUP(int) {
    SPDLOG_INFO("📶 SIGHUP received → reloading symbol config...");
    if (!ConfigLoader::reloadConfig()) {
        SPDLOG_ERROR("❌ Reload failed.");
    }
}

int main() {
    SPDLOG_INFO("🛠️ Starting config-service...");

    try {
        // Step 0: Register signal handler
        std::signal(SIGHUP, handleSIGHUP);  // 🔁 Add this

        // Step 1: Load .env
        ConfigLoader::loadEnv(".env");

        // Step 2: Get path to symbol config JSON
        std::string configPath = ConfigLoader::getEnv("CONFIG_JSON_PATH");
        if (configPath.empty()) {
            SPDLOG_CRITICAL("CONFIG_JSON_PATH not found in .env");
            return EXIT_FAILURE;
        }

        // Step 3: Load symbol configs
        if (!ConfigLoader::loadConfig(configPath)) {
            SPDLOG_CRITICAL("Failed to load config JSON: {}", configPath);
            return EXIT_FAILURE;
        }

        // Step 4: Get port
        std::string port = ConfigLoader::getEnv("GRPC_PORT");
        if (port.empty()) port = "50051";
        std::string address = "0.0.0.0:" + port;

        // Step 5: Start gRPC server
        ConfigGRPCServer server;
        server.run(address);
    } catch (const std::exception& e) {
        SPDLOG_CRITICAL("Unhandled exception: {}", e.what());
        return EXIT_FAILURE;
    } catch (...) {
        SPDLOG_CRITICAL("Unknown fatal error occurred.");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
