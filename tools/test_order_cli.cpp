#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

int main() {
    std::string orderId, symbol;
    double price, qty;

    std::cout << "Enter Order ID: ";
    std::cin >> orderId;
    std::cout << "Enter Symbol (e.g., BTCUSDT): ";
    std::cin >> symbol;
    std::cout << "Enter Price: ";
    std::cin >> price;
    std::cout << "Enter Quantity: ";
    std::cin >> qty;

    json order = {
        {"orderId", orderId},
        {"symbol", symbol},
        {"price", price},
        {"qty", qty}
    };

    std::ofstream out("order_payload.json");
    out << order.dump(2);
    out.close();

    std::cout << "Order saved to order_payload.json\n";
    std::cout << "Now run: ./order_service < order_payload.json\n";
    return 0;
}
