#include "risk_evaluator.hpp"
#include "liquidation_manager.hpp"
#include <spdlog/spdlog.h>

void RiskEvaluator::evaluate(double ltp) {
    SPDLOG_INFO("Evaluating risk based on LTP: {}", ltp);

    // Placeholder: risk rules
    bool risky = (ltp < 25000); 

    if (risky) {
        LiquidationManager liquidator;
        liquidator.liquidate("user_001", ltp);
    }
}