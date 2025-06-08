#include "matcher.hpp"

std::vector<Trade> Matcher::submit(const Order& order) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto& book = books_[order.symbol];

    if (!book) {
        book = std::make_shared<OrderBook>(order.symbol);
    }

    return book->processOrder(order);
}

bool Matcher::cancel(const std::string& symbol, const std::string& orderId) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!books_.count(symbol)) return false;
    return books_[symbol]->cancelOrder(orderId);
}
