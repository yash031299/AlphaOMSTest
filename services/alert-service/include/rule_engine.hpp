#pragma once
#include "alert.hpp"
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

struct AlertContext {
    std::string userId;
    std::string symbol;
    double balance = 0;
    double equity = 0;
    double pnl = 0;
    double margin = 0;
    std::string timestamp;
};

class RuleEngine {
public:
    explicit RuleEngine(const std::string& configPath);
    std::vector<Alert> evaluate(const AlertContext& ctx);

private:
    nlohmann::json rules_;
};
