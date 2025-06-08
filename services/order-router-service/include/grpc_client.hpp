#pragma once
#include <string>

class ExecutionGRPCClient {
public:
    ExecutionGRPCClient(const std::string& target);
    bool sendOrder(const std::string& json);
};
