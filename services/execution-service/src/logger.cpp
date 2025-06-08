#include "logger.hpp"
#include <spdlog/spdlog.h>

void ExecutionLogger::log(const std::string& orderJson, const std::string& type) {
    SPDLOG_INFO("Execution log: [{}] {}", type, orderJson);
}
