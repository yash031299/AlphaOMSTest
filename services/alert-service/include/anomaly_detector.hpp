#pragma once
#include "rule_engine.hpp"
#include "alert_dispatcher.hpp"
#include <boost/asio.hpp>
#include <memory>
#include <vector>

class AnomalyDetector {
public:
    AnomalyDetector(boost::asio::io_context& io,
                    std::shared_ptr<RuleEngine> ruleEngine,
                    std::shared_ptr<AlertDispatcher> dispatcher);

    void start();
    void evaluate(const std::string& userId, const std::string& metric, double value); // ✅ Declare

private:
    void poll();

    boost::asio::io_context& io_;
    std::shared_ptr<RuleEngine> ruleEngine_;
    std::shared_ptr<AlertDispatcher> dispatcher_;
    std::shared_ptr<boost::asio::steady_timer> timer_;  // ✅ Declare
};
