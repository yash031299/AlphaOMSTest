#pragma once

#include "ledger.hpp"
#include "postgres_writer.hpp"
#include <boost/asio.hpp>
#include <memory>
#include <string>

class SettlementEngine {
public:
    SettlementEngine(boost::asio::io_context& io,
                     std::shared_ptr<Ledger> ledger,
                     int intervalSeconds);

    void start();
    void performSettlement();

private:
    void scheduleNext();

    boost::asio::io_context& io_;
    std::shared_ptr<Ledger> ledger_;
    int interval_;
    std::shared_ptr<boost::asio::steady_timer> timer_;

    PostgresWriter postgresWriter_;
};
