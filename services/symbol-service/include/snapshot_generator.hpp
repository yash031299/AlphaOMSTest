#ifndef SNAPSHOT_GENERATOR_HPP
#define SNAPSHOT_GENERATOR_HPP

#include "orderbook_manager.hpp"
#include <string>
#include <vector>

class SnapshotGenerator {
public:
    static std::string generateJsonL1(const std::string& symbol,
                                      const std::vector<OrderLevel>& levels);
};

#endif // SNAPSHOT_GENERATOR_HPP
