#ifndef SETTLEMENT_ENGINE_HPP
#define SETTLEMENT_ENGINE_HPP

#include "ledger.hpp"
#include <string>
#include <memory>
#include <functional>
#include <boost/asio.hpp>

class SettlementEngine {
public:
    SettlementEngine(boost::asio::io_context& io,
                     std::shared_ptr<Ledger> ledger,
                     int intervalSeconds = 86400); // default 1 day

    void start();

private:
    void scheduleNext();
    void performSettlement();

    boost::asio::io_context& io_;
    std::shared_ptr<Ledger> ledger_;
    int interval_;
    std::shared_ptr<boost::asio::steady_timer> timer_;
};

#endif // SETTLEMENT_ENGINE_HPP
