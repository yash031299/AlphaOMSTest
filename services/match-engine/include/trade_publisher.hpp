#ifndef TRADE_PUBLISHER_HPP
#define TRADE_PUBLISHER_HPP

#include "book.hpp"
#include <vector>

class TradePublisher {
public:
    void publish(const std::vector<Trade>& trades);
};

#endif // TRADE_PUBLISHER_HPP
