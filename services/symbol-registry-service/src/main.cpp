#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

extern void startGrpcServer(const std::string& address);

int main() {
    auto logger = spdlog::stdout_color_mt("symbol-registry");
    spdlog::set_default_logger(logger);
    spdlog::set_level(spdlog::level::info);
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%^%l%$] %v");

    SPDLOG_INFO("🚀 Starting Symbol Registry Service...");
    startGrpcServer("0.0.0.0:6000");

    return 0;
}
