#pragma once
#include <string>

class AccountingGRPCServer {
public:
    void run(const std::string& address = "0.0.0.0:5050");
};
