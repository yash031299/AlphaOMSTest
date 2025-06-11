#ifndef ACCOUNTING_LEDGER_HPP
#define ACCOUNTING_LEDGER_HPP

#include <string>
#include <unordered_map>
#include <mutex>
#include <vector>

struct LedgerEntry {
    std::string userId;
    std::string symbol;
    double amount;
    std::string type;      // deposit, trade, funding, liquidation, fee
    std::string reference; // orderId, txId, etc.
    std::string timestamp;
};

class Ledger {
public:
    void credit(const LedgerEntry& entry);
    void debit(const LedgerEntry& entry);
    std::vector<LedgerEntry> getEntries(const std::string& userId);
    std::unordered_map<std::string, std::vector<LedgerEntry>> getAll() const;


private:
    std::mutex mutex_;
    std::unordered_map<std::string, std::vector<LedgerEntry>> ledger_;
};

#endif // ACCOUNTING_LEDGER_HPP
