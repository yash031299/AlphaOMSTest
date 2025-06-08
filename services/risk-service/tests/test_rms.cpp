#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "rms_engine.hpp"
#include "user_state.hpp"

TEST_CASE("RMS Engine PnL and Margin Calculation") {
    UserState state;
    state.userId = "test-user";
    state.entryPrice = 20000;
    state.positionSize = 1;
    state.side = +1;
    state.leverage = 10;
    state.walletBalance = 1000;

    RMSEngine rms(state);

    SUBCASE("PnL is positive when LTP > entry") {
        rms.onPriceUpdate(21000);
        CHECK(rms.getUnrealizedPnL() > 0);
    }

    SUBCASE("PnL is negative when LTP < entry") {
        rms.onPriceUpdate(19000);
        CHECK(rms.getUnrealizedPnL() < 0);
    }

    SUBCASE("Triggers liquidation if equity < maintenance") {
        rms.onPriceUpdate(18000);  // force liquidation
        CHECK(rms.shouldLiquidate());  // assume method exists or add it for test
    }
}
