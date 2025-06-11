#include "rule_engine.hpp"
#include <fstream>
#include <spdlog/spdlog.h>

RuleEngine::RuleEngine(const std::string& configPath) {
    try {
        std::ifstream file(configPath);
        if (!file.is_open()) {
            SPDLOG_ERROR("RuleEngine: Failed to open config file: {}", configPath);
            return;
        }

        file >> rules_;

        if (!rules_.is_array()) {
            SPDLOG_ERROR("RuleEngine: rule config must be a JSON array.");
            throw std::runtime_error("Invalid rule config format");
        }

        SPDLOG_INFO("✅ Loaded {} alert rules.", rules_.size());
    } catch (const std::exception& e) {
        SPDLOG_ERROR("RuleEngine config error: {}", e.what());
    }
}

std::vector<Alert> RuleEngine::evaluate(const AlertContext& ctx) {
    std::vector<Alert> alerts;

    for (const auto& rule : rules_) {
        try {
            if (!rule.contains("type") || !rule.contains("symbol") || !rule.contains("threshold"))
                continue;

            std::string type = rule.value("type", "");
            std::string symbol = rule.value("symbol", "");
            double threshold = rule.value("threshold", 0.0);
            std::string severity_str = rule.value("severity", "LOW");
            AlertSeverity severity = from_string(severity_str);

            if (!symbol.empty() && symbol != ctx.symbol) continue;

            if (type == "low_balance" && ctx.balance < threshold) {
                alerts.push_back({
                    .metric = "balance",
                    .value = ctx.balance,
                    .threshold = threshold,
                    .severity = severity,
                    .message = "Balance low: " + std::to_string(ctx.balance),
                    .timestamp = ctx.timestamp
                });
            } else if (type == "high_pnl" && ctx.pnl > threshold) {
                alerts.push_back({
                    .metric = "pnl",
                    .value = ctx.pnl,
                    .threshold = threshold,
                    .severity = severity,
                    .message = "PNL unusually high: " + std::to_string(ctx.pnl),
                    .timestamp = ctx.timestamp
                });
            } else if (type == "liquidation_margin" && ctx.margin < threshold) {
                alerts.push_back({
                    .metric = "margin",
                    .value = ctx.margin,
                    .threshold = threshold,
                    .severity = severity,
                    .message = "Margin below liquidation threshold: " + std::to_string(ctx.margin),
                    .timestamp = ctx.timestamp
                });
            }
        } catch (const std::exception& e) {
            SPDLOG_ERROR("Rule evaluation error: {}", e.what());
        }
    }

    return alerts;
}
