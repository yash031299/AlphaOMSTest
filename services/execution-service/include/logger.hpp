#pragma once
#include <string>

class ExecutionLogger {
public:
    static void log(const std::string& orderJson, const std::string& type);
};
