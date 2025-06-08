#include "symbol_client.hpp"
#include <spdlog/spdlog.h>

SymbolClient::SymbolClient(std::shared_ptr<grpc::Channel> channel)
    : stub_(symbol::SymbolService::NewStub(channel)) {}

std::optional<SymbolMeta> SymbolClient::getSymbolMeta(const std::string &symbol_name) {
    symbol::SymbolRequest request;
    request.set_symbol(symbol_name);

    symbol::SymbolResponse response;
    grpc::ClientContext context;

    grpc::Status status = stub_->GetSymbol(&context, request, &response);

    if (!status.ok()) {
        SPDLOG_ERROR("SymbolClient gRPC failed: {}", status.error_message());
        return std::nullopt;
    }

    if (!response.found()) {
        SPDLOG_WARN("Symbol not found in registry: {}", symbol_name);
        return std::nullopt;
    }

    SymbolMeta meta;
    meta.symbol = response.symbol();
    meta.tick_size = response.tick_size();
    meta.max_leverage = response.max_leverage();
    meta.price_precision = response.price_precision();
    meta.quantity_precision = response.quantity_precision();

    return meta;
}
