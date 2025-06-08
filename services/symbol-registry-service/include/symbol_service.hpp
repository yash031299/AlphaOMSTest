#ifndef SYMBOL_SERVICE_HPP
#define SYMBOL_SERVICE_HPP

#include "symbol.grpc.pb.h"
#include <unordered_map>
#include <string>
#include <mutex>

struct SymbolConfig {
    double tickSize;
    int precision;
    double maxLeverage;
    double initialMargin;
    double maintenanceMargin;
};

class SymbolServiceImpl final : public SymbolRegistry::Service {
public:
    SymbolServiceImpl();

    grpc::Status GetSymbolMetadata(grpc::ServerContext* context,
                                   const SymbolRequest* request,
                                   SymbolMetadata* response) override;

    bool loadSymbolsFromFile(const std::string& filepath = "config/symbols.json");

private:
    std::unordered_map<std::string, SymbolConfig> symbols_;
    std::mutex mutex_;
};

#endif // SYMBOL_SERVICE_HPP
