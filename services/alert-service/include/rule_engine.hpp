#ifndef RULE_ENGINE_HPP
#define RULE_ENGINE_HPP

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

struct AlertContext {
    std::string userId;
    std::string symbol;
    double balance;
    double equity;
    double pnl;
    double margin;
    std::string timestamp;
};

struct Alert {
    std::string message;
    std::string severity; // info, warning, critical
    std::string timestamp;
};

class RuleEngine {
public:
    RuleEngine(const std::string& configPath);

    std::vector<Alert> evaluate(const AlertContext& ctx);

private:
    nlohmann::json rules_;
};

#endif // RULE_ENGINE_HPP
