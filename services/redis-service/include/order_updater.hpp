#pragma once
#include <string>

class OrderUpdater {
public:
    void updateStatus(const std::string& orderId, const std::string& status);
};
