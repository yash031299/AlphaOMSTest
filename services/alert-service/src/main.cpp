#include "rule_engine.hpp"
#include "anomaly_detector.hpp"
#include "alert_dispatcher.hpp"
#include "redis_alert_listener.hpp"
#include "redis_anomaly_listener.hpp"


#include <boost/asio.hpp>
#include <spdlog/spdlog.h>
#include <thread>


RedisAlertListener alertListener;
RedisAnomalyListener anomalyListener;


int main() {
    spdlog::set_level(spdlog::level::info);
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%^%l%$] %v");

    SPDLOG_INFO("🚨 Alert Service Booting Up...");

    boost::asio::io_context io;
    auto ruleEngine = std::make_shared<RuleEngine>("config/alert_rules.json");
    auto dispatcher = std::make_shared<AlertDispatcher>();
    alertListener.start();

    auto detector = std::make_shared<AnomalyDetector>(io, ruleEngine, dispatcher);

    detector->start();
    anomalyListener.start();

    io.run();

    return 0;
}
