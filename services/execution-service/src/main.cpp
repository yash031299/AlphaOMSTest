#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/security/server_credentials.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <cstdlib>
#include <fstream>
#include <thread>
#include <chrono>
#include "grpc_server.hpp"

// Simple healthcheck HTTP server
void startHealthCheckServer(int port) {
    std::thread([port]() {
        SPDLOG_INFO("⚕️  Healthcheck running at http://0.0.0.0:{}/healthz", port);
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        sockaddr_in addr {};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = INADDR_ANY;
        bind(sockfd, (sockaddr*)&addr, sizeof(addr));
        listen(sockfd, 5);

        while (true) {
            int client = accept(sockfd, nullptr, nullptr);
            const char* resp =
                "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nOK";
            send(client, resp, strlen(resp), 0);
            close(client);
        }
    }).detach();
}

void setupLogging() {
    auto logger = spdlog::stdout_color_mt("console");
    spdlog::set_default_logger(logger);
    spdlog::set_level(spdlog::level::info);
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%^%l%$] %v");
}

void loadEnv(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        size_t delimPos = line.find('=');
        if (delimPos != std::string::npos) {
            std::string key = line.substr(0, delimPos);
            std::string val = line.substr(delimPos + 1);
            setenv(key.c_str(), val.c_str(), 1);
        }
    }
}

int main() {
    setupLogging();
    try {
        loadEnv(".env");

        std::string port = std::getenv("EXECUTION_GRPC_PORT") ? std::getenv("EXECUTION_GRPC_PORT") : "50051";
        std::string server_address = "0.0.0.0:" + port;

        ExecutionServiceImpl service;
        grpc::ServerBuilder builder;

        // TLS placeholder (mTLS will be added post wiring)
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
        builder.RegisterService(&service);

        std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
        SPDLOG_INFO("🚀 execution-service started on {}", server_address);

        // Start healthcheck on separate port (e.g., 8080)
        startHealthCheckServer(8080);

        server->Wait();
    } catch (const std::exception& ex) {
        SPDLOG_CRITICAL("❌ Uncaught exception in execution-service: {}", ex.what());
    } catch (...) {
        SPDLOG_CRITICAL("❌ Unknown fatal error in execution-service");
    }
    return 0;
}
