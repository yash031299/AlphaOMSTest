#include "orderbook_manager.hpp"

void OrderBookManager::addOrder(const std::string& symbol, const Order& order) {
    std::lock_guard<std::mutex> lock(booksMutex_);
    auto& book = books_[symbol];

    std::lock_guard<std::mutex> bookLock(book.mutex);
    book.orders[order.orderId] = order;

    auto& side = order.isBuy ? book.bids : book.asks;
    side[order.price] += order.quantity;
}

void OrderBookManager::cancelOrder(const std::string& symbol, const std::string& orderId) {
    std::lock_guard<std::mutex> lock(booksMutex_);
    auto& book = books_[symbol];

    std::lock_guard<std::mutex> bookLock(book.mutex);
    auto it = book.orders.find(orderId);
    if (it == book.orders.end()) return;

    const Order& order = it->second;
    auto& side = order.isBuy ? book.bids : book.asks;
    side[order.price] -= order.quantity;
    if (side[order.price] <= 0) side.erase(order.price);

    book.orders.erase(it);
}

void OrderBookManager::modifyOrder(const std::string& symbol, const Order& newOrder) {
    cancelOrder(symbol, newOrder.orderId);
    addOrder(symbol, newOrder);
}

std::vector<OrderLevel> OrderBookManager::getTopOfBook(const std::string& symbol, size_t depth) {
    std::lock_guard<std::mutex> lock(booksMutex_);
    auto& book = books_[symbol];
    std::lock_guard<std::mutex> bookLock(book.mutex);

    std::vector<OrderLevel> snapshot;

    auto addTopLevels = [&](const auto& side, bool isBid) {
        size_t count = 0;
        for (const auto& [price, qty] : side) {
            snapshot.push_back({price, qty});
            if (++count >= depth) break;
        }
    };

    addTopLevels(book.bids, true);
    addTopLevels(book.asks, false);
    return snapshot;
}
