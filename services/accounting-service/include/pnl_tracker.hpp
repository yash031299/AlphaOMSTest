#ifndef PNL_TRACKER_HPP
#define PNL_TRACKER_HPP

#include "ledger.hpp"
#include <string>
#include <memory>

class PnLTracker {
public:
    PnLTracker(std::shared_ptr<Ledger> ledger);

    void recordRealizedPnL(const std::string& userId,
                           const std::string& symbol,
                           double pnl,
                           const std::string& timestamp,
                           const std::string& reference = "realized_pnl");

private:
    std::shared_ptr<Ledger> ledger_;
};

#endif // PNL_TRACKER_HPP
