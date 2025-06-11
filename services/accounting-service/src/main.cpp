#include "ledger.hpp"
#include "redis_listener.hpp"
#include "settlement_engine.hpp"
#include "metrics_server.hpp"
#include "grpc_interface.hpp"
#include <csignal>
#include <atomic>

#include <boost/asio.hpp>
#include <csignal>
#include <memory>
#include <spdlog/spdlog.h>
#include <cstdlib>
#include <thread>

std::shared_ptr<SettlementEngine> global_settlement;
std::unique_ptr<AccountingGRPCServer> grpc_server;
std::unique_ptr<RedisListener> redis_listener;

void handleShutdown(int signum) {
    SPDLOG_WARN("🛑 Received signal {} – flushing snapshot and exiting...", signum);
    if (global_settlement) global_settlement->performSettlement();
    std::exit(0);
}

int main() {
    SPDLOG_INFO("🚀 Starting accounting-service...");

    // Load environment
    const char* grpc_host = std::getenv("GRPC_HOST") ?: "0.0.0.0";
    const char* grpc_port = std::getenv("GRPC_PORT") ?: "6008";
    const char* interval_env = std::getenv("SNAPSHOT_INTERVAL_SEC") ?: "60";
    const char* redis_flag = std::getenv("ENABLE_REDIS_EVENTS");

    std::string grpc_address = std::string(grpc_host) + ":" + std::string(grpc_port);
    int snapshot_interval = std::stoi(interval_env);
    bool enable_redis = redis_flag && std::string(redis_flag) == "true";

    // Core shared ledger
    auto ledger = std::make_shared<Ledger>();

    // Start Boost ASIO context with thread pool
    boost::asio::io_context io;
    std::vector<std::thread> io_threads;
    const int thread_count = std::thread::hardware_concurrency();

    // Redis listener (optional)
    if (enable_redis) {
        redis_listener = std::make_unique<RedisListener>("localhost", 6379, ledger);
        redis_listener->start();
        SPDLOG_INFO("📡 Redis listener active");
    }

    // Start settlement engine (async)
    global_settlement = std::make_shared<SettlementEngine>(io, ledger, snapshot_interval);
    global_settlement->start();

    // Start Prometheus metrics + admin API
    auto metrics = std::make_shared<MetricsServer>(ledger, 8081);
    metrics->start();

    // Start gRPC service
    grpc_server = std::make_unique<AccountingGRPCServer>(ledger);
    std::thread grpc_thread([&]() {
        grpc_server->run(grpc_address);
    });

    // Register signal handler
    std::signal(SIGINT, handleShutdown);
    std::signal(SIGTERM, handleShutdown);

    // Launch I/O thread pool
    for (int i = 0; i < thread_count; ++i) {
        io_threads.emplace_back([&io]() { io.run(); });
    }

    

    // Wait for all threads
    grpc_thread.join();
    for (auto& t : io_threads) t.join();

    return 0;
}
