#include "grpc_interface.hpp"
#include "symbol_client.hpp"  // gRPC client implementation

PnLServiceImpl::PnLServiceImpl() {
    symbolClient_ = std::make_shared<SymbolServiceClient>("localhost:6000");
}

grpc::Status PnLServiceImpl::GetEquity(grpc::ServerContext*,
                                       const PnLRequest* request,
                                       PnLResponse* response) {
    const std::string& userId = request->user_id();

    if (!users_.count(userId)) {
        users_[userId] = std::make_shared<EquityCalculator>(userId, 1000.0);  // initial wallet
        users_[userId]->setSymbolService(symbolClient_);
    }

    double equity = users_[userId]->getEquity();
    response->set_user_id(userId);
    response->set_equity(equity);
    return grpc::Status::OK;
}

grpc::Status PnLServiceImpl::CheckLiquidation(grpc::ServerContext*,
                                              const PnLRequest* request,
                                              LiquidationResponse* response) {
    const std::string& userId = request->user_id();

    if (!users_.count(userId)) {
        users_[userId] = std::make_shared<EquityCalculator>(userId, 1000.0);
        users_[userId]->setSymbolService(symbolClient_);
    }

    bool liquidate = users_[userId]->isLiquidatable();
    response->set_user_id(userId);
    response->set_should_liquidate(liquidate);
    return grpc::Status::OK;
}

std::shared_ptr<EquityCalculator> PnLServiceImpl::getOrCreateUser(const std::string& userId) {
    if (!users_.count(userId)) {
        users_[userId] = std::make_shared<EquityCalculator>(userId, 1000.0);
        users_[userId]->setSymbolService(symbolClient_);
    }
    return users_[userId];
}
