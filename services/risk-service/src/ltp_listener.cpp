#include "ltp_listener.hpp"
#include "risk_evaluator.hpp"
#include <spdlog/spdlog.h>

void LTPListener::onPriceUpdate(double ltp) {
    SPDLOG_INFO("LTP Updated: {}", ltp);
    RiskEvaluator evaluator;
    evaluator.evaluate(ltp);
}
