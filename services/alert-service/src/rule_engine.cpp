#include "rule_engine.hpp"
#include <fstream>
#include <spdlog/spdlog.h>

RuleEngine::RuleEngine(const std::string& configPath) {
    std::ifstream file(configPath);
    if (!file.is_open()) {
        SPDLOG_ERROR("RuleEngine: Failed to open rule config file.");
        return;
    }
    file >> rules_;
    SPDLOG_INFO("Loaded {} alert rules.", rules_.size());
}

std::vector<Alert> RuleEngine::evaluate(const AlertContext& ctx) {
    std::vector<Alert> alerts;

    for (const auto& rule : rules_) {
        std::string type = rule.value("type", "");
        std::string symbol = rule.value("symbol", "");
        double threshold = rule.value("threshold", 0.0);

        if (!symbol.empty() && symbol != ctx.symbol) continue;

        if (type == "low_balance" && ctx.balance < threshold) {
            alerts.push_back({
                .message = "Balance low: " + std::to_string(ctx.balance),
                .severity = "warning",
                .timestamp = ctx.timestamp
            });
        } else if (type == "high_pnl" && ctx.pnl > threshold) {
            alerts.push_back({
                .message = "PNL unusually high: " + std::to_string(ctx.pnl),
                .severity = "info",
                .timestamp = ctx.timestamp
            });
        } else if (type == "liquidation_margin" && ctx.margin < threshold) {
            alerts.push_back({
                .message = "Margin below liquidation threshold: " + std::to_string(ctx.margin),
                .severity = "critical",
                .timestamp = ctx.timestamp
            });
        }
    }

    return alerts;
}
