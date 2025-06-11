#include "grpc_server.hpp"
#include "redis_consumer.hpp"
#include <spdlog/spdlog.h>
#include <thread>
#include <csignal>

RedisConsumer* global_consumer = nullptr;

// Handle Ctrl+C to cleanly stop Redis consumer
void signalHandler(int signum) {
    SPDLOG_WARN("Interrupt signal ({}) received. Shutting down...", signum);
    if (global_consumer) {
        global_consumer->stop();
    }
    std::exit(signum);
}

int main() {
    SPDLOG_INFO("🔐 Starting audit-log-service (gRPC + Redis mode)...");

    
    // Register signal handler for graceful shutdown
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    RedisConsumer consumer("localhost", 6379, "AUDIT:LOGS");
    global_consumer = &consumer;
    consumer.start();

    GRPCServer server;
    server.run();  // blocking call

    consumer.stop();
    return 0;
}
