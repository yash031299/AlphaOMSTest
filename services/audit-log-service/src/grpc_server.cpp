#include "grpc_server.hpp"
#include "log_writer_async.hpp"
#include "log_event.pb.h"
#include "log_event.grpc.pb.h"
#include <grpcpp/grpcpp.h>
#include <spdlog/spdlog.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

class LogEventServiceImpl final : public LogEventService::Service {
public:
    grpc::Status LogTrade(grpc::ServerContext*, const TradeEvent* request, LogAck* reply) override {
        try {
            TradeEvent log = *request;
            AsyncLogWriter::get().enqueue(log);
            reply->set_success(true);
            reply->set_message("Trade logged.");
        } catch (const std::exception& e) {
            SPDLOG_ERROR("Exception in LogTrade: {}", e.what());
            reply->set_success(false);
            reply->set_message(e.what());
        }
        return grpc::Status::OK;
    }

    grpc::Status LogOrder(grpc::ServerContext*, const OrderEvent* request, LogAck* reply) override {
        try {
            OrderEvent log = *request;
            AsyncLogWriter::get().enqueue(log);
            reply->set_success(true);
            reply->set_message("Order logged.");
        } catch (const std::exception& e) {
            SPDLOG_ERROR("Exception in LogOrder: {}", e.what());
            reply->set_success(false);
            reply->set_message(e.what());
        }
        return grpc::Status::OK;
    }

    grpc::Status LogFunding(grpc::ServerContext*, const FundingEvent* request, LogAck* reply) override {
        try {
            FundingEvent log = *request;
            AsyncLogWriter::get().enqueue(log);
            reply->set_success(true);
            reply->set_message("Funding logged.");
        } catch (const std::exception& e) {
            SPDLOG_ERROR("Exception in LogFunding: {}", e.what());
            reply->set_success(false);
            reply->set_message(e.what());
        }
        return grpc::Status::OK;
    }

    grpc::Status LogLiquidation(grpc::ServerContext*, const LiquidationEvent* request, LogAck* reply) override {
        try {
            LiquidationEvent log = *request;
            AsyncLogWriter::get().enqueue(log);
            reply->set_success(true);
            reply->set_message("Liquidation logged.");
        } catch (const std::exception& e) {
            SPDLOG_ERROR("Exception in LogLiquidation: {}", e.what());
            reply->set_success(false);
            reply->set_message(e.what());
        }
        return grpc::Status::OK;
    }
};

void GRPCServer::run(const std::string& address) {
    try {
        grpc::EnableDefaultHealthCheckService(true);
        grpc::reflection::InitProtoReflectionServerBuilderPlugin();

        LogEventServiceImpl service;
        grpc::ServerBuilder builder;
        builder.AddListeningPort(address, grpc::InsecureServerCredentials());
        builder.RegisterService(&service);

        std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
        SPDLOG_INFO("🛰️  Audit gRPC server started at {}", address);
        server->Wait();
    } catch (const std::exception& e) {
        SPDLOG_ERROR("Failed to start GRPC server: {}", e.what());
    }
}
