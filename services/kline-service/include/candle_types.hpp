#ifndef CANDLE_TYPES_HPP
#define CANDLE_TYPES_HPP

#include <string>
#include <cstdint>

struct TradeTick {
    std::string symbol;
    double price;
    double quantity;
    std::string timestamp; // ISO format (e.g., "2025-06-07 22:01:05")
};

struct Candle {
    std::string symbol;
    std::string interval;   // "1m", "5m", "1h"
    std::string startTime;  // rounded ISO timestamp (e.g., "2025-06-07 22:01:00")

    double open = 0.0;
    double high = 0.0;
    double low = 0.0;
    double close = 0.0;
    double volume = 0.0;
    uint32_t tradeCount = 0;
};

#endif // CANDLE_TYPES_HPP
