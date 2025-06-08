#include "funding_handler.hpp"

FundingHandler::FundingHandler(std::shared_ptr<Ledger> ledger)
    : ledger_(ledger) {}

void FundingHandler::applyFunding(const std::string& userId,
                                  const std::string& symbol,
                                  double fundingPnL,
                                  const std::string& timestamp,
                                  const std::string& reference) {
    LedgerEntry entry;
    entry.userId = userId;
    entry.symbol = symbol;
    entry.amount = fundingPnL;
    entry.type = "funding";
    entry.reference = reference;
    entry.timestamp = timestamp;

    if (fundingPnL >= 0)
        ledger_->credit(entry);
    else
        ledger_->debit(entry);
}
