#include "rms_engine.hpp"
#include "user_state.hpp"

#include <boost/asio.hpp>
#include <hiredis/hiredis.h>
#include <spdlog/spdlog.h>
#include <memory>
#include <unordered_map>
#include <chrono>


int startHealthCheckServer();
MarginListener marginListener;

using boost::asio::steady_timer;
using namespace std::chrono_literals;

boost::asio::io_context io;
boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work = boost::asio::make_work_guard(io);
boost::asio::thread_pool pool(4);  // You can tune to 4–16 threads depending on CPU


// Simulated user list
std::vector<std::string> users = { "user-001", "user-002", "user-003" };

// RMS handler per user
std::unordered_map<std::string, std::shared_ptr<RMSEngine>> rmsMap;

// Redis poll + RMS evaluate per user
void scheduleRMSLoop(const std::string& userId) {
    auto timer = std::make_shared<steady_timer>(io, 1s);
    timer->async_wait([timer, userId](const boost::system::error_code& ec) {
        if (ec) return;

        double ltp = 0.0;
        

        redisContext* ctx = safeRedisConnect("127.0.0.1", 6379);
        if (!ctx || ctx->err) {
            SPDLOG_ERROR("[{}] Redis connection error: {}", userId, ctx ? ctx->errstr : "null");
            return;
        }

        redisReply* reply = safeRedisCommand(ctx,
            "XREVRANGE market_ticks + - COUNT 1");
        if (reply && reply->type == REDIS_REPLY_ARRAY && reply->elements > 0) {
            auto fields = reply->element[0]->element[1];
            for (size_t j = 0; j < fields->elements; j += 2) {
                std::string key = fields->element[j]->str;
                std::string val = fields->element[j + 1]->str;
                if (key == "ltp") ltp = std::stod(val);
                
            }
        }

        if (reply) freeReplyObject(reply);
        redisFree(ctx);
        if (ltp == 0.0) return;  // 🛡️ guard empty or invalid price
        // Feed LTP to RMS
        rmsMap[userId]->onPriceUpdate(ltp);
        rmsMap[userId]->syncMargin();


        timer->expires_after(1s);
        // Reschedule
        scheduleRMSLoop(userId);
    });
}

int main() {
    SPDLOG_INFO("Starting RMS Risk Engine with Boost Asio...");

    for (const auto& userId : users) {
        UserState state;
        state.userId = userId;
        rmsMap[userId] = std::make_shared<RMSEngine>(state);

        scheduleRMSLoop(userId);
    }
    marginListener.start();

    std::thread([] {
        using namespace std::chrono_literals;
        while (true) {
            std::this_thread::sleep_for(10s);
            const auto& metrics = RMSMetrics::get();
            SPDLOG_INFO("[MONITOR] Liquidations: {}, Users: {}",
                        metrics.liquidationCount.load(), metrics.lastEquity.size());
            for (const auto& [user, equity] : metrics.lastEquity) {
                SPDLOG_INFO("[EQUITY] {}: {:.2f}", user, equity);
            }
        }
    }).detach();
    
    std::thread healthThread([] {
        startHealthCheckServer();  // Starts /healthz on :8082
    });
    healthThread.detach();

    
    for (int i = 0; i < 4; ++i) {
        boost::asio::post(pool, [&]() { io.run(); });
    }


    return 0;
}
