#pragma once
#include "alert.hpp"
#include <string>
#include <sqlite3.h>

class AlertAuditLogger {
public:
    AlertAuditLogger(const std::string& dbPath);
    ~AlertAuditLogger();

    void log(const std::string& userId, const Alert& alert);

private:
    void init();                 // 👈 Add this
    std::string dbPath_;        // 👈 Add this
    sqlite3* db_;
};
