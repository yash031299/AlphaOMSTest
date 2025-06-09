#include "grpc_interface.hpp"
#include "funding_handler.hpp"
#include "pnl_tracker.hpp"
#include "accounting.grpc.pb.h"
#include <grpcpp/grpcpp.h>
#include <spdlog/spdlog.h>

static FundingHandler fundingHandler;
static PnLTracker pnlTracker;

class AccountingServiceImpl final : public accounting::AccountingService::Service {
public:
    grpc::Status ApplyFunding(grpc::ServerContext*,
                              const accounting::FundingRequest* req,
                              accounting::Ack* res) override {
        fundingHandler.applyFunding(req->user_id(), req->symbol(), req->funding_amount());
        res->set_success(true);
        res->set_message("Funding applied");
        return grpc::Status::OK;
    }

    grpc::Status RecordPnL(grpc::ServerContext*,
                           const accounting::PnLRequest* req,
                           accounting::Ack* res) override {
        pnlTracker.recordPnL(req->user_id(), req->symbol(), req->realized_pnl());
        res->set_success(true);
        res->set_message("PnL recorded");
        return grpc::Status::OK;
    }
};

void AccountingGRPCServer::run(const std::string& address) {
    AccountingServiceImpl service;
    grpc::ServerBuilder builder;
    builder.AddListeningPort(address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());

    SPDLOG_INFO("📘 Accounting gRPC server listening on {}", address);
    server->Wait();
}
