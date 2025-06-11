#pragma once
#include <pqxx/pqxx>
#include <string>
#include "ledger.hpp"

class PostgresWriter {
public:
    PostgresWriter(const std::string& uri);
    void persistSnapshot(const Ledger& ledger);

private:
    pqxx::connection conn_;
};
