#include "log_writer.hpp"
#include "log_event.pb.h"
#include <cassert>
#include <iostream>

int main() {
    LogWriter writer("data/logs_test");

    // TradeEvent test
    TradeEvent t;
    t.set_symbol("BTCUSDT");
    t.set_makerorderid("mak123");
    t.set_takerorderid("tak123");
    t.set_price(26200.12);
    t.set_quantity(0.5);
    t.set_timestamp("2025-06-07T10:00:00Z");

    std::string jsonStr = writer.toJson(t);
    std::cout << "Trade JSON:\n" << jsonStr << "\n";

    // OrderEvent test
    OrderEvent o;
    o.set_orderid("ord001");
    o.set_userid("usr001");
    o.set_symbol("ETHUSDT");
    o.set_side("buy");
    o.set_type_detail("limit");
    o.set_price(1850.25);
    o.set_quantity(1.0);
    o.set_status("filled");
    o.set_timestamp("2025-06-07T11:00:00Z");

    std::cout << "Order JSON:\n" << writer.toJson(o) << "\n";

    // FundingEvent test
    FundingEvent f;
    f.set_symbol("XRPUSDT");
    f.set_fundingrate(0.00015);
    f.set_longoi(10000);
    f.set_shortoi(9000);
    f.set_timestamp("2025-06-07T12:00:00Z");

    std::cout << "Funding JSON:\n" << writer.toJson(f) << "\n";

    // LiquidationEvent test
    LiquidationEvent l;
    l.set_userid("liq001");
    l.set_symbol("ADAUSDT");
    l.set_pnl(-150.75);
    l.set_reason("margin");
    l.set_timestamp("2025-06-07T13:00:00Z");

    std::cout << "Liquidation JSON:\n" << writer.toJson(l) << "\n";

    std::cout << "✅ All log_writer tests completed.\n";
    return 0;
}
