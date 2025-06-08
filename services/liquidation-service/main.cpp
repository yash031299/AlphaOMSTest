#include "include/liquidation_service.hpp"
#include "include/user_manage.h"
#include "include/redis_update.h"

int main() {
    auto redisPool = std::make_shared<RedisConnectionPool>("127.0.0.1", 6379, 10, nullptr);
    auto usrMng = std::make_shared<userManager>(5, redisPool, nullptr);
    LiquidationService service(redisPool, usrMng);

    service.start();
    while (true) std::this_thread::sleep_for(std::chrono::seconds(10));
}
