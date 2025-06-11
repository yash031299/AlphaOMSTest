#include "alert_audit_logger.hpp"
#include <spdlog/spdlog.h>

AlertAuditLogger::AlertAuditLogger(const std::string& dbPath)
    : dbPath_(dbPath) {
    init();
}
AlertAuditLogger::~AlertAuditLogger() {
    if (db_) {
        sqlite3_close(db_);
        db_ = nullptr;
    }
}


void AlertAuditLogger::init() {
    sqlite3* db;
    if (sqlite3_open(dbPath_.c_str(), &db) != SQLITE_OK) {
        SPDLOG_ERROR("SQLite open failed: {}", sqlite3_errmsg(db));
        return;
    }

    const char* ddl = "CREATE TABLE IF NOT EXISTS alerts ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "userId TEXT, metric TEXT, value REAL, threshold REAL,"
                      "severity TEXT, message TEXT, timestamp TEXT);";

    char* errMsg = nullptr;
    if (sqlite3_exec(db, ddl, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        SPDLOG_ERROR("SQLite table creation failed: {}", errMsg);
        sqlite3_free(errMsg);
    }

    sqlite3_close(db);
}

void AlertAuditLogger::log(const std::string& userId, const Alert& alert) {
    sqlite3* db;
    if (sqlite3_open(dbPath_.c_str(), &db) != SQLITE_OK) return;

    std::string sql = "INSERT INTO alerts (userId, metric, value, threshold, severity, message, timestamp) "
                      "VALUES (?, ?, ?, ?, ?, ?, ?);";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, userId.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, alert.metric.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_double(stmt, 3, alert.value);
        sqlite3_bind_double(stmt, 4, alert.threshold);
        sqlite3_bind_text(stmt, 5, to_string(alert.severity).c_str(), -1, SQLITE_STATIC); // ✅ Fix
        sqlite3_bind_text(stmt, 6, alert.message.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 7, alert.timestamp.c_str(), -1, SQLITE_STATIC);
        sqlite3_step(stmt);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
