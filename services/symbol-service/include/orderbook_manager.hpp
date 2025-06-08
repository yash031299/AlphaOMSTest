#ifndef ORDERBOOK_MANAGER_HPP
#define ORDERBOOK_MANAGER_HPP

#include <map>
#include <string>
#include <mutex>
#include <unordered_map>
#include <vector>

struct Order {
    std::string orderId;
    double price;
    double quantity;
    bool isBuy;
};

struct OrderLevel {
    double price;
    double quantity;
};

class OrderBookManager {
public:
    void addOrder(const std::string& symbol, const Order& order);
    void cancelOrder(const std::string& symbol, const std::string& orderId);
    void modifyOrder(const std::string& symbol, const Order& order);

    std::vector<OrderLevel> getTopOfBook(const std::string& symbol, size_t depth = 1);

private:
    struct Book {
        std::map<double, double, std::greater<>> bids; // price → qty
        std::map<double, double> asks;                 // price → qty
        std::unordered_map<std::string, Order> orders;
        std::mutex mutex;
    };

    std::unordered_map<std::string, Book> books_;
    std::mutex booksMutex_;
};

#endif // ORDERBOOK_MANAGER_HPP
