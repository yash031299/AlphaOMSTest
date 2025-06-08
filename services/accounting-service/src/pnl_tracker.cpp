#include "pnl_tracker.hpp"

PnLTracker::PnLTracker(std::shared_ptr<Ledger> ledger)
    : ledger_(ledger) {}

void PnLTracker::recordRealizedPnL(const std::string& userId,
                                   const std::string& symbol,
                                   double pnl,
                                   const std::string& timestamp,
                                   const std::string& reference) {
    LedgerEntry entry;
    entry.userId = userId;
    entry.symbol = symbol;
    entry.amount = pnl;
    entry.type = "realized_pnl";
    entry.reference = reference;
    entry.timestamp = timestamp;

    if (pnl >= 0)
        ledger_->credit(entry);
    else
        ledger_->debit(entry);
}
