#include "include/order_router.hpp"
#include "include/user_manage.h"
#include "include/redis_update.h"

int main() {
    auto redisPool = std::make_shared<RedisConnectionPool>("127.0.0.1", 6379, 5, nullptr);
    auto userMng = std::make_shared<userManager>(5, redisPool, nullptr);
    OrderRouterService router(redisPool, userMng);

    router.start();
    while (true) std::this_thread::sleep_for(std::chrono::seconds(10));
}
