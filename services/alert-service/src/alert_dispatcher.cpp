#include "alert_dispatcher.hpp"
#include <spdlog/spdlog.h>

void AlertDispatcher::dispatch(const std::string& userId, const Alert& alert) {
    std::string tag = "[ALERT/" + alert.severity + "]";
    SPDLOG_WARN("{} [{}] {} → {}", tag, alert.timestamp, userId, alert.message);

    // Future extensions:
    // - Send to Redis pub/sub
    // - Trigger webhook to alerting system
    // - Write to alert-audit database
    // - Push to PagerDuty or Slack
}
