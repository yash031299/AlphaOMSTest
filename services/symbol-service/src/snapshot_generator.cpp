#include "snapshot_generator.hpp"
#include <sstream>
#include <iomanip>

std::string SnapshotGenerator::generateJsonL1(const std::string& symbol,
                                              const std::vector<OrderLevel>& levels) {
    std::ostringstream oss;
    oss << "{";
    oss << "\"symbol\":\"" << symbol << "\",";
    oss << "\"bid\":";
    if (!levels.empty()) {
        oss << std::fixed << std::setprecision(2)
            << "{\"price\":" << levels[0].price << ",\"qty\":" << levels[0].quantity << "}";
    } else {
        oss << "null";
    }
    oss << ",\"ask\":";
    if (levels.size() > 1) {
        oss << std::fixed << std::setprecision(2)
            << "{\"price\":" << levels[1].price << ",\"qty\":" << levels[1].quantity << "}";
    } else {
        oss << "null";
    }
    oss << "}";
    return oss.str();
}
