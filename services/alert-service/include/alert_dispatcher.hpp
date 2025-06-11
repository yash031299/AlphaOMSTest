#pragma once
#include "alert.hpp"
#include <memory>
#include <string>

class AlertAuditLogger;

class AlertDispatcher {
public:
    AlertDispatcher(std::shared_ptr<AlertAuditLogger> logger);
    void dispatch(const std::string& userId, const Alert& alert);  // ✅ Corrected signature

private:
    std::shared_ptr<AlertAuditLogger> logger_;
};
