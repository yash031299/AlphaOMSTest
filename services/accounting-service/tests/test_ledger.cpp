#include "ledger.hpp"
#include <cassert>
#include <iostream>

int main() {
    Ledger ledger;

    LedgerEntry e1{"user1", "BTC", 100.0, "deposit", "tx123", "2025-06-10T12:00:00Z"};
    LedgerEntry e2{"user1", "BTC", 50.0, "funding", "fund456", "2025-06-10T12:01:00Z"};

    ledger.credit(e1);
    ledger.credit(e2);

    auto entries = ledger.getEntries("user1");

    assert(entries.size() == 2);
    assert(entries[0].amount == 100.0);
    assert(entries[1].reference == "fund456");

    std::cout << "✅ test_ledger passed\n";
    return 0;
}
