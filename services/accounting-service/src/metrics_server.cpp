#include "metrics_server.hpp"
#include "httplib.h"
#include <sstream>
#include <spdlog/spdlog.h>
#include <hiredis/hiredis.h>
#include <pqxx/pqxx>


MetricsServer::MetricsServer(std::shared_ptr<Ledger> ledger, int port)
    : ledger_(ledger), port_(port), running_(false) {}

void MetricsServer::start() {
    running_ = true;
    server_thread_ = std::thread([this]() {
        httplib::Server svr;

        svr.Get("/health", [](const httplib::Request&, httplib::Response& res) {
            res.set_content("OK", "text/plain");
        });

        svr.Get("/metrics", [this](const httplib::Request&, httplib::Response& res) {
            std::ostringstream out;
            auto snapshot = ledger_->getAll();

            size_t entry_count = 0;
            for (const auto& [_, entries] : snapshot) {
                entry_count += entries.size();
            }

            out << "# HELP accounting_ledger_entries_total Total ledger entries\n";
            out << "# TYPE accounting_ledger_entries_total gauge\n";
            out << "accounting_ledger_entries_total " << entry_count << "\n";
            out << "accounting_settlement_total " << snapshot_count.load() << "\n";

            res.set_content(out.str(), "text/plain");
        });

        svr.Get("/ledger/latest", [this](const httplib::Request&, httplib::Response& res) {
            std::ostringstream json;
            json << "[";
            bool first = true;
            for (const auto& [user, entries] : ledger_->getAll()) {
                if (entries.empty()) continue;
                const auto& e = entries.back();
                if (!first) json << ",";
                json << "{"
                    << "\"user\":\"" << user << "\","
                    << "\"symbol\":\"" << e.symbol << "\","
                    << "\"amount\":" << e.amount << ","
                    << "\"type\":\"" << e.type << "\","
                    << "\"ref\":\"" << e.reference << "\","
                    << "\"ts\":\"" << e.timestamp << "\"}";
                first = false;
            }
            json << "]";
            res.set_content(json.str(), "application/json");
        });


        svr.Get("/ledger/summary", [this](const httplib::Request&, httplib::Response& res) {
            std::map<std::string, std::map<std::string, double>> summary;
            for (const auto& [user, entries] : ledger_->getAll()) {
                for (const auto& e : entries) {
                    summary[user][e.symbol] += e.amount;
                }
            }
            std::ostringstream json;
            json << "{";
            bool first_user = true;
            for (const auto& [user, balances] : summary) {
                if (!first_user) json << ",";
                json << "\"" << user << "\":{";
                bool first_sym = true;
                for (const auto& [sym, amt] : balances) {
                    if (!first_sym) json << ",";
                    json << "\"" << sym << "\":" << amt;
                    first_sym = false;
                }
                json << "}";
                first_user = false;
            }
            json << "}";
            res.set_content(json.str(), "application/json");
        });

        svr.Get("/health/deep", [](const httplib::Request&, httplib::Response& res) {
            bool redis_ok = false, pg_ok = false;
            std::string redis_msg = "❌", pg_msg = "❌";

            redisContext* rctx = redisConnect("localhost", 6379);
            if (rctx && !rctx->err) {
                redis_ok = true;
                redis_msg = "✅";
                redisFree(rctx);
            }

            try {
                pqxx::connection conn(std::getenv("POSTGRES_URI"));
                if (conn.is_open()) {
                    pg_ok = true;
                    pg_msg = "✅";
                }
            } catch (...) {}

            std::ostringstream body;
            body << "{ \"redis\": \"" << redis_msg << "\", \"postgres\": \"" << pg_msg << "\" }";
            res.set_content(body.str(), "application/json");
            res.status = (redis_ok && pg_ok) ? 200 : 503;
        });


        SPDLOG_INFO("📊 Metrics server started at http://0.0.0.0:{}", port_);
        svr.listen("0.0.0.0", port_);
    });

}

void MetricsServer::stop() {
    running_ = false;
    if (server_thread_.joinable()) server_thread_.join();
}
