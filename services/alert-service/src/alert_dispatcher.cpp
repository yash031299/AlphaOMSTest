#include "alert_dispatcher.hpp"
#include "alert_audit_logger.hpp"
#include "spdlog/spdlog.h"

AlertDispatcher::AlertDispatcher(std::shared_ptr<AlertAuditLogger> logger)
    : logger_(logger) {}

void AlertDispatcher::dispatch(const std::string& userId, const Alert& alert) {
    spdlog::info("[DISPATCH] Alert for user {}: {} [{}]",
                 userId, alert.message, static_cast<int>(alert.severity));

    if (logger_) {
        logger_->log(userId, alert);
    }
}
