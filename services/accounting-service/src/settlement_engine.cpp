#include "settlement_engine.hpp"
#include "alert_utils.hpp"
#include <spdlog/spdlog.h>
#include <chrono>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <filesystem>
#include <thread>
#include <hiredis/hiredis.h>

SettlementEngine::SettlementEngine(boost::asio::io_context& io,
                                   std::shared_ptr<Ledger> ledger,
                                   int intervalSeconds)
    : io_(io), ledger_(ledger), interval_(intervalSeconds),
      postgresWriter_(std::getenv("POSTGRES_URI")) {
    timer_ = std::make_shared<boost::asio::steady_timer>(io_);
}

void SettlementEngine::start() {
    scheduleNext();
}

void SettlementEngine::scheduleNext() {
    timer_->expires_after(std::chrono::seconds(interval_));
    timer_->async_wait([this](const boost::system::error_code& ec) {
        if (!ec) {
            performSettlement();
            scheduleNext();
        }
    });
}


void SettlementEngine::performSettlement() {
    SPDLOG_INFO("⏳ Performing PnL settlement and ledger snapshot...");

    // -- Snapshot file path from .env
    std::string dir = std::getenv("LEDGER_SNAPSHOT_DIR") ?: "data/snapshots";
    std::filesystem::create_directories(dir);
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);

    std::stringstream filename;
    filename << dir << "/ledger_" << std::put_time(std::localtime(&t), "%Y%m%d_%H%M%S") << ".csv";

    // -- Write CSV snapshot
    std::ofstream out(filename.str());
    if (!out.is_open()) {
        SPDLOG_ERROR("❌ Failed to open CSV snapshot file: {}", filename.str());
        publishAlert("Failed to open CSV snapshot file: " + filename.str());
        return;
    }

    out << "userId,symbol,amount,type,reference,timestamp\n";
    for (const auto& [userId, entries] : ledger_->getAll()) {
        for (const auto& e : entries) {
            out << e.userId << "," << e.symbol << "," << e.amount << "," << e.type << ","
                << e.reference << "," << e.timestamp << "\n";
        }
    }
    out.close();
    SPDLOG_INFO("✅ CSV snapshot written to {}", filename.str());

    // -- PostgreSQL persistence
    postgresWriter_.persistSnapshot(*ledger_);

    // -- Retention policy (keep last 10 snapshots)
    namespace fs = std::filesystem;
    std::vector<fs::directory_entry> snapshots;
    for (const auto& f : fs::directory_iterator(dir)) {
        if (f.path().extension() == ".csv") {
            snapshots.push_back(f);
        }
    }
    std::sort(snapshots.begin(), snapshots.end(), [](const auto& a, const auto& b) {
        return fs::last_write_time(a) > fs::last_write_time(b); // Newest first
    });

    for (size_t i = 10; i < snapshots.size(); ++i) {
        fs::remove(snapshots[i]);
        SPDLOG_INFO("🗑️ Removed old snapshot: {}", snapshots[i].path().string());
    }

    // -- Optional: S3 Upload
    const char* s3flag = std::getenv("ENABLE_S3_UPLOAD");
    const char* bucket = std::getenv("S3_BUCKET");

    if (s3flag && std::string(s3flag) == "true" && bucket) {
        std::string cmd = "aws s3 cp " + filename.str() + " s3://" + bucket + "/ledger_snapshots/";
        int status = std::system(cmd.c_str());
        if (status == 0) {
            SPDLOG_INFO("☁️ Uploaded snapshot to S3 bucket: {}", bucket);
        } else {
            SPDLOG_WARN("⚠️ S3 upload failed with status {}", status);
            publishAlert("S3 upload failed for file: " + filename.str());
        }
    }
}
