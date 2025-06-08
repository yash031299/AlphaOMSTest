#include "ledger.hpp"

void Ledger::credit(const LedgerEntry& entry) {
    std::lock_guard<std::mutex> lock(mutex_);
    ledger_[entry.userId].push_back(entry);
}

void Ledger::debit(const LedgerEntry& entry) {
    std::lock_guard<std::mutex> lock(mutex_);
    ledger_[entry.userId].push_back(entry);
}

std::vector<LedgerEntry> Ledger::getEntries(const std::string& userId) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (ledger_.count(userId)) {
        return ledger_[userId];
    }
    return {};
}
