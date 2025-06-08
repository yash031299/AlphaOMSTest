#include "book.hpp"
#include <chrono>
#include <sstream>

OrderBook::OrderBook(const std::string& symbol)
    : symbol_(symbol) {}

bool OrderBook::cancelOrder(const std::string& orderId) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (orderIndex_.find(orderId) == orderIndex_.end()) return false;

    auto [isBuy, price] = orderIndex_[orderId];
    auto& side = isBuy ? bids_ : asks_;

    auto it = side.find(price);
    if (it != side.end()) {
        auto& queue = it->second;
        for (auto qIt = queue.begin(); qIt != queue.end(); ++qIt) {
            if (qIt->orderId == orderId) {
                queue.erase(qIt);
                orderIndex_.erase(orderId);
                if (queue.empty()) side.erase(price);
                return true;
            }
        }
    }

    return false;
}

std::vector<Trade> OrderBook::processOrder(const Order& order) {
    std::lock_guard<std::mutex> lock(mutex_);
    std::vector<Trade> trades;
    auto incoming = order;

    auto& opposite = incoming.isBuy ? asks_ : bids_;
    auto comparator = incoming.isBuy ? std::less_equal<>() : std::greater_equal<>();

    for (auto it = opposite.begin(); it != opposite.end() && incoming.quantity > 0;) {
        double levelPrice = it->first;

        if (!comparator(incoming.price, levelPrice)) break;

        auto& queue = it->second;
        while (!queue.empty() && incoming.quantity > 0) {
            Order& resting = queue.front();
            double tradedQty = std::min(incoming.quantity, resting.quantity);

            trades.push_back({
                .makerOrderId = resting.orderId,
                .takerOrderId = incoming.orderId,
                .symbol = symbol_,
                .price = levelPrice,
                .quantity = tradedQty,
                .timestamp = "TODO_NOW" // stubbed
            });

            resting.quantity -= tradedQty;
            incoming.quantity -= tradedQty;

            if (resting.quantity <= 0) {
                orderIndex_.erase(resting.orderId);
                queue.pop_front();
            }
        }

        if (queue.empty()) {
            it = opposite.erase(it);
        } else {
            ++it;
        }
    }

    // Add remaining as limit order
    if (incoming.quantity > 0) {
        auto& sameSide = incoming.isBuy ? bids_ : asks_;
        sameSide[incoming.price].push_back(incoming);
        orderIndex_[incoming.orderId] = {incoming.isBuy, incoming.price};
    }

    return trades;
}
