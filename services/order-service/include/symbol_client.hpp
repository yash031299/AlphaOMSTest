#pragma once

#include <memory>
#include <string>
#include <optional>
#include <grpcpp/grpcpp.h>
#include "symbol.grpc.pb.h"

struct SymbolMeta {
    std::string symbol;
    double tick_size;
    double max_leverage;
    int price_precision;
    int quantity_precision;
};

class SymbolClient {
public:
    explicit SymbolClient(std::shared_ptr<grpc::Channel> channel);

    // Returns nullopt if symbol is not found
    std::optional<SymbolMeta> getSymbolMeta(const std::string &symbol_name);

private:
    std::unique_ptr<symbol::SymbolService::Stub> stub_;
};
