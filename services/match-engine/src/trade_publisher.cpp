#include "trade_publisher.hpp"
#include <spdlog/spdlog.h>

void TradePublisher::publish(const std::vector<Trade>& trades) {
    for (const auto& t : trades) {
        SPDLOG_INFO("[TRADE] {} | {} @ {:.2f} Qty: {} | Maker: {} | Taker: {}",
                    t.symbol, t.timestamp, t.price, t.quantity,
                    t.makerOrderId, t.takerOrderId);
    }
}
