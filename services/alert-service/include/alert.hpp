#pragma once
#include <string>
#include <unordered_map>

enum class AlertSeverity {
    LOW,
    MEDIUM,
    HIGH,
    CRITICAL
};

inline std::string to_string(AlertSeverity severity) {
    switch (severity) {
        case AlertSeverity::LOW: return "LOW";
        case AlertSeverity::MEDIUM: return "MEDIUM";
        case AlertSeverity::HIGH: return "HIGH";
        case AlertSeverity::CRITICAL: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

inline AlertSeverity from_string(const std::string& s) {
    static const std::unordered_map<std::string, AlertSeverity> map = {
        {"LOW", AlertSeverity::LOW},
        {"MEDIUM", AlertSeverity::MEDIUM},
        {"HIGH", AlertSeverity::HIGH},
        {"CRITICAL", AlertSeverity::CRITICAL}
    };
    auto it = map.find(s);
    return (it != map.end()) ? it->second : AlertSeverity::LOW;
}

struct Alert {
    std::string metric;
    double value = 0;
    double threshold = 0;
    AlertSeverity severity = AlertSeverity::LOW;
    std::string message;
    std::string timestamp;
};
