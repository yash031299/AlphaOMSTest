#include "grpc_server.hpp"
#include "log_writer.hpp"
#include "event_types.hpp"
#include "log_event.grpc.pb.h"
#include <grpcpp/grpcpp.h>
#include <spdlog/spdlog.h>

class LogEventServiceImpl final : public LogEventService::Service {
public:
    grpc::Status LogTrade(grpc::ServerContext*,
                          const TradeEvent* request,
                          LogAck* reply) override {
        TradeLog log = {
            request->symbol(),
            request->makerorderid(),
            request->takerorderid(),
            request->price(),
            request->quantity(),
            request->timestamp()
        };

        LogWriter::get().writeTrade(log);
        reply->set_success(true);
        reply->set_message("Trade logged.");
        return grpc::Status::OK;
    }

    grpc::Status LogOrder(grpc::ServerContext*,
                          const OrderEvent* request,
                          LogAck* reply) override {
        OrderLog log = {
            request->orderid(),
            request->userid(),
            request->symbol(),
            request->side(),
            request->type_detail(),
            request->price(),
            request->quantity(),
            request->status(),
            request->timestamp()
        };

        LogWriter::get().writeOrder(log);
        reply->set_success(true);
        reply->set_message("Order logged.");
        return grpc::Status::OK;
    }

    grpc::Status LogFunding(grpc::ServerContext*,
                            const FundingEvent* request,
                            LogAck* reply) override {
        FundingLog log = {
            request->symbol(),
            request->fundingrate(),
            request->longoi(),
            request->shortoi(),
            request->timestamp()
        };

        LogWriter::get().writeFunding(log);
        reply->set_success(true);
        reply->set_message("Funding logged.");
        return grpc::Status::OK;
    }

    grpc::Status LogLiquidation(grpc::ServerContext*,
                                const LiquidationEvent* request,
                                LogAck* reply) override {
        LiquidationLog log = {
            request->userid(),
            request->symbol(),
            request->pnl(),
            request->reason(),
            request->timestamp()
        };

        LogWriter::get().writeLiquidation(log);
        reply->set_success(true);
        reply->set_message("Liquidation logged.");
        return grpc::Status::OK;
    }
};

void GRPCServer::run(const std::string& address) {
    LogEventServiceImpl service;
    grpc::ServerBuilder builder;
    builder.AddListeningPort(address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    SPDLOG_INFO("🛰️  Audit gRPC server started at {}", address);
    server->Wait();
}
