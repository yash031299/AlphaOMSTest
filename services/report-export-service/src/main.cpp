#include "report_generator.hpp"
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <filesystem>

int main() {
    spdlog::set_level(spdlog::level::info);
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%^%l%$] %v");

    SPDLOG_INFO("📄 Report Export Service Starting...");

    std::filesystem::create_directories("data/exports");
    ReportGenerator generator("data/exports");

    nlohmann::json mockData = {
        {
            {"timestamp", "2025-06-07 18:01:00"},
            {"type", "realized_pnl"},
            {"symbol", "BTC-USDT"},
            {"amount", 52.13}
        },
        {
            {"timestamp", "2025-06-07 18:10:00"},
            {"type", "funding"},
            {"symbol", "BTC-USDT"},
            {"amount", -2.45}
        },
        {
            {"timestamp", "2025-06-07 18:20:00"},
            {"type", "liquidation"},
            {"symbol", "ETH-USDT"},
            {"amount", -150.00}
        }
    };

    generator.generateCSVReport("user-001", mockData, "ledger");
    generator.generatePDFReport("user-001", mockData, "ledger");

    SPDLOG_INFO("📁 Export complete. Files saved in data/exports/");
    return 0;
}
