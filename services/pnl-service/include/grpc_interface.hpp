#ifndef GRPC_INTERFACE_HPP
#define GRPC_INTERFACE_HPP

#include <grpcpp/grpcpp.h>
#include "pnl_engine.hpp"
#include "equity_calculator.hpp"
#include "symbol.grpc.pb.h"
#include "pnl.grpc.pb.h"

class PnLServiceImpl final : public PnLService::Service {
public:
    PnLServiceImpl();

    grpc::Status GetEquity(grpc::ServerContext* context,
                           const PnLRequest* request,
                           PnLResponse* response) override;

    grpc::Status CheckLiquidation(grpc::ServerContext* context,
                                  const PnLRequest* request,
                                  LiquidationResponse* response) override;

private:
    std::unordered_map<std::string, std::shared_ptr<EquityCalculator>> users_;
    std::shared_ptr<SymbolServiceClient> symbolClient_;  // shared client to registry
};

#endif // GRPC_INTERFACE_HPP
