#include "rule_engine.hpp"
#include "alert_dispatcher.hpp"
#include "redis_alert_listener.hpp"
#include "redis_anomaly_listener.hpp"
#include "metrics.hpp"
#include "health_check.hpp"
#include "alert_audit_logger.hpp"
#include "anomaly_detector.hpp"
#include "config_loader.hpp"

#include <boost/asio.hpp>
#include <memory>
#include <thread>
#include <spdlog/spdlog.h>

// 👇 Define global anomalyDetector
std::shared_ptr<AnomalyDetector> anomalyDetector;

int main() {
    try {
        SPDLOG_INFO("🚀 Starting Alert Service");

        std::string rulePath = getEnvVar("ALERT_RULES", "config/alert_rules.json");
        std::string dbPath = getEnvVar("SQLITE_DB", "alerts.db");

        boost::asio::io_context io;

        auto ruleEngine = std::make_shared<RuleEngine>(rulePath);
        auto auditLogger = std::make_shared<AlertAuditLogger>(dbPath);
        auto dispatcher = std::make_shared<AlertDispatcher>(auditLogger);

        anomalyDetector = std::make_shared<AnomalyDetector>(io, ruleEngine, dispatcher); // ✅ fix
        anomalyDetector->start();

        RedisAlertListener alertListener;
        alertListener.start(dispatcher);

        RedisAnomalyListener anomalyListener(dispatcher);
        anomalyListener.start();

        startMetricsServer(9090);
        startHealthCheckServer(8086);

        SPDLOG_INFO("📡 Services running...");
        io.run();  // Instead of sleep loop, use Asio event loop
    } catch (const std::exception& ex) {
        SPDLOG_ERROR("💥 Fatal Exception: {}", ex.what());
    } catch (...) {
        SPDLOG_ERROR("💥 Unknown exception occurred.");
    }

    return 0;
}
