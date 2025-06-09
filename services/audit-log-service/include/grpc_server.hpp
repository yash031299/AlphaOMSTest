#pragma once
#include <memory>

class GRPCServer {
public:
    void run(const std::string& address = "0.0.0.0:9090");
};
