#include "ledger.hpp"

void Ledger::credit(const LedgerEntry& entry) {
    ledger_[entry.userId].push_back(entry);
}

void Ledger::debit(const LedgerEntry& entry) {
    ledger_[entry.userId].push_back(entry);
}

std::vector<LedgerEntry> Ledger::getEntries(const std::string& userId) {
    if (ledger_.count(userId)) {
        return ledger_[userId];
    }
    return {};
}

std::unordered_map<std::string, std::vector<LedgerEntry>> Ledger::getAll() const {
    return ledger_;
}
