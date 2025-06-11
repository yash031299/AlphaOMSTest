#include "postgres_writer.hpp"
#include "alert_utils.hpp"
#include <thread>  // for sleep
#include <chrono>
#include <spdlog/spdlog.h>
#include <hiredis/hiredis.h>  // required for Redis alert


PostgresWriter::PostgresWriter(const std::string& uri)
    : conn_(uri) {}


void PostgresWriter::persistSnapshot(const Ledger& ledger) {
    bool success = false;

    for (int attempt = 1; attempt <= 3; ++attempt) {
        try {
            pqxx::work txn(conn_);

            for (const auto& [userId, entries] : ledger.getAll()) {
                for (const auto& e : entries) {
                    txn.exec_params(
                        "INSERT INTO ledger_entries(user_id, symbol, amount, type, reference, timestamp) VALUES ($1, $2, $3, $4, $5, $6)",
                        e.userId, e.symbol, e.amount, e.type, e.reference, e.timestamp
                    );


                }
            }

            txn.commit();
            SPDLOG_INFO("✅ Persisted ledger snapshot to PostgreSQL (attempt {})", attempt);
            success = true;
            break;

        } catch (const std::exception& e) {
            SPDLOG_WARN("⚠️ PostgreSQL snapshot failed (attempt {}): {}", attempt, e.what());
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    }

    if (!success) {
        SPDLOG_ERROR("🔥 All retries to write ledger snapshot failed.");
        publishAlert("PostgreSQL snapshot failed after 3 attempts");
    }
}
