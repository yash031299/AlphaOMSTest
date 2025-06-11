#pragma once
#include <memory>
#include <string>
#include "ledger.hpp"

class AccountingGRPCServer {
public:
    AccountingGRPCServer(std::shared_ptr<Ledger> ledger);
    void run(const std::string& address);
private:
    std::shared_ptr<Ledger> ledger_;
};
