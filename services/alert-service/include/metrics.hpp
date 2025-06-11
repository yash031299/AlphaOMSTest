#pragma once
#include <atomic>

inline std::atomic<int> alert_triggered_total{0};
inline std::atomic<int> anomaly_detected_total{0};
void startMetricsServer(unsigned short port = 8082);