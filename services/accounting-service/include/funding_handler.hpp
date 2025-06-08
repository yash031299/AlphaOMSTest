#ifndef FUNDING_HANDLER_HPP
#define FUNDING_HANDLER_HPP

#include "ledger.hpp"
#include <string>
#include <memory>

class FundingHandler {
public:
    FundingHandler(std::shared_ptr<Ledger> ledger);

    void applyFunding(const std::string& userId,
                      const std::string& symbol,
                      double fundingPnL,
                      const std::string& timestamp,
                      const std::string& reference = "funding");

private:
    std::shared_ptr<Ledger> ledger_;
};

#endif // FUNDING_HANDLER_HPP