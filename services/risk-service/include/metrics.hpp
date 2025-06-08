#pragma once
#include <atomic>
#include <unordered_map>
#include <string>

class RMSMetrics {
public:
    std::atomic<int> liquidationCount{0};
    std::unordered_map<std::string, double> lastEquity;

    static RMSMetrics& get() {
        static RMSMetrics instance;
        return instance;
    }

    void reportEquity(const std::string& userId, double equity) {
        lastEquity[userId] = equity;
    }

    void incrementLiquidation() {
        liquidationCount.fetch_add(1);
    }
};
