#include "../include/rule_engine.hpp"
#include <cassert>
#include <iostream>

int main() {
    RuleEngine engine("config/alert_rules.json");

    AlertContext ctx;
    ctx.userId = "test-user";
    ctx.symbol = "BTC";
    ctx.balance = 80.0;
    ctx.equity = 120.0;
    ctx.pnl = 700.0;
    ctx.margin = 30.0;
    ctx.timestamp = "now";

    auto alerts = engine.evaluate(ctx);
    for (const auto& a : alerts) {
        std::cout << "[TEST] Alert: " << a.message << " | Severity: " << to_string(a.severity) << "\n";
    }

    assert(!alerts.empty());
    std::cout << "[PASS] RuleEngine test passed.\n";
    return 0;
}
