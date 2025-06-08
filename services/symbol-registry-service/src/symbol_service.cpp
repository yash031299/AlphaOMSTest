#include "symbol_service.hpp"
#include <fstream>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

using json = nlohmann::json;

SymbolServiceImpl::SymbolServiceImpl() {
    loadSymbolsFromFile();
}

bool SymbolServiceImpl::loadSymbolsFromFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        SPDLOG_ERROR("Failed to open symbol config: {}", filepath);
        return false;
    }

    json j;
    file >> j;

    std::lock_guard<std::mutex> lock(mutex_);
    for (auto& [symbol, config] : j.items()) {
        symbols_[symbol] = SymbolConfig{
            config["tick_size"].get<double>(),
            config["precision"].get<int>(),
            config["max_leverage"].get<double>(),
            config["initial_margin"].get<double>(),
            config["maintenance_margin"].get<double>()
        };
    }

    SPDLOG_INFO("Loaded {} symbols from config", symbols_.size());
    return true;
}

grpc::Status SymbolServiceImpl::GetSymbolMetadata(grpc::ServerContext*,
                                                  const SymbolRequest* request,
                                                  SymbolMetadata* response) {
    std::string symbol = request->symbol();
    std::lock_guard<std::mutex> lock(mutex_);

    if (symbols_.find(symbol) == symbols_.end()) {
        return grpc::Status(grpc::StatusCode::NOT_FOUND, "Symbol not found");
    }

    const auto& cfg = symbols_[symbol];
    response->set_symbol(symbol);
    response->set_tick_size(cfg.tickSize);
    response->set_precision(cfg.precision);
    response->set_max_leverage(cfg.maxLeverage);
    response->set_initial_margin(cfg.initialMargin);
    response->set_maintenance_margin(cfg.maintenanceMargin);

    return grpc::Status::OK;
}
