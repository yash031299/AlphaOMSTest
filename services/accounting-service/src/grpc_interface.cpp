#include "grpc_interface.hpp"
#include "funding_handler.hpp"
#include "pnl_tracker.hpp"
#include "accounting.grpc.pb.h"
#include <grpcpp/grpcpp.h>
#include <spdlog/spdlog.h>

class AccountingServiceImpl final : public accounting::AccountingService::Service {
public:
    AccountingServiceImpl(std::shared_ptr<Ledger> ledger)
        : fundingHandler_(ledger), pnlTracker_(ledger) {}

    grpc::Status ApplyFunding(grpc::ServerContext*,
                              const accounting::FundingRequest* req,
                              accounting::Ack* res) override {
        fundingHandler_.applyFunding(req->user_id(), req->symbol(),
                                     req->funding_amount(), req->timestamp(), req->reference());
        res->set_success(true);
        res->set_message("Funding applied");
        return grpc::Status::OK;
    }

    grpc::Status RecordPnL(grpc::ServerContext*,
                           const accounting::PnLRequest* req,
                           accounting::Ack* res) override {
        pnlTracker_.recordRealizedPnL(req->user_id(), req->symbol(),
                                      req->realized_pnl(), req->timestamp(), req->reference());
        res->set_success(true);
        res->set_message("PnL recorded");
        return grpc::Status::OK;
    }

private:
    FundingHandler fundingHandler_;
    PnLTracker pnlTracker_;
};

AccountingGRPCServer::AccountingGRPCServer(std::shared_ptr<Ledger> ledger)
    : ledger_(ledger) {}

void AccountingGRPCServer::run(const std::string& address) {
    AccountingServiceImpl service(ledger_);
    grpc::ServerBuilder builder;
    builder.AddListeningPort(address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());

    SPDLOG_INFO("📘 Accounting gRPC server listening on {}", address);
    server->Wait();
}
