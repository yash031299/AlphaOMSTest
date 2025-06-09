#pragma once
#include <string>

class ConfigGRPCServer {
public:
    void run(const std::string& address = "0.0.0.0:50051");
};
