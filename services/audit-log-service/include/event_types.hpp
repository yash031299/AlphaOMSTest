#ifndef AUDIT_EVENT_TYPES_HPP
#define AUDIT_EVENT_TYPES_HPP

#include <string>

struct TradeEvent {
    std::string symbol;
    std::string makerOrderId;
    std::string takerOrderId;
    double price;
    double quantity;
    std::string timestamp;
};

struct OrderEvent {
    std::string orderId;
    std::string userId;
    std::string symbol;
    std::string side;     // buy/sell
    std::string type;     // limit/market
    double price;
    double quantity;
    std::string status;   // filled, partial, cancelled
    std::string timestamp;
};

struct FundingEvent {
    std::string symbol;
    double fundingRate;
    double longOi;
    double shortOi;
    std::string timestamp;
};

struct LiquidationEvent {
    std::string userId;
    std::string symbol;
    double pnl;
    std::string reason;
    std::string timestamp;
};

#endif // AUDIT_EVENT_TYPES_HPP
