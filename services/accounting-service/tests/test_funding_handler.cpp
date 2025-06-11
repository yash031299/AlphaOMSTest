#include "ledger.hpp"
#include "funding_handler.hpp"
#include <cassert>
#include <iostream>
#include <memory>

int main() {
    auto ledger = std::make_shared<Ledger>();
    FundingHandler handler(ledger);

    handler.applyFunding("user42", "ETH", 25.0, "2025-06-10T12:05:00Z", "fund123");

    auto entries = ledger->getEntries("user42");

    assert(entries.size() == 1);
    assert(entries[0].amount == 25.0);
    assert(entries[0].symbol == "ETH");

    std::cout << "✅ test_funding_handler passed\n";
    return 0;
}
