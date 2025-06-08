#ifndef ALERT_DISPATCHER_HPP
#define ALERT_DISPATCHER_HPP

#include "rule_engine.hpp"
#include <string>

class AlertDispatcher {
public:
    void dispatch(const std::string& userId, const Alert& alert);
};

#endif // ALERT_DISPATCHER_HPP
