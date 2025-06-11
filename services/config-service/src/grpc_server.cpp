#include "grpc_server.hpp"
#include "config_loader.hpp"
#include "config.grpc.pb.h"
#include <grpcpp/grpcpp.h>
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>
#include <grpcpp/ext/proto_server_reflection_plugin.h>


class ConfigServiceImpl final : public config::ConfigService::Service {
public:
    grpc::Status GetConfig(grpc::ServerContext*,
                           const config::ConfigRequest* request,
                           config::ConfigValue* response) override {
        try {
            auto json = ConfigLoader::getSymbolConfig(request->key());
            if (json.empty()) {
                response->set_found(false);
                response->set_value("");
            } else {
                response->set_found(true);
                response->set_value(json.dump());  // Send full JSON as string
            }
            return grpc::Status::OK;
        } catch (const std::exception& e) {
            SPDLOG_ERROR("Exception in GetConfig: {}", e.what());
            return grpc::Status(grpc::StatusCode::INTERNAL, "Server error");
        }
    }

    grpc::Status GetAll(grpc::ServerContext*,
                        const config::Empty*,
                        config::ConfigMap* response) override {
        try {
            auto allEnv = ConfigLoader::getAllEnv();
            auto* map = response->mutable_configs();
            for (const auto& [k, v] : allEnv) {
                (*map)[k] = v;
            }
            return grpc::Status::OK;
        } catch (const std::exception& e) {
            SPDLOG_ERROR("Exception in GetAll: {}", e.what());
            return grpc::Status(grpc::StatusCode::INTERNAL, "Server error");
        }
    }
};

void ConfigGRPCServer::run(const std::string& address) {
    try {
        ConfigServiceImpl service;
        grpc::ServerBuilder builder;
        builder.AddListeningPort(address, grpc::InsecureServerCredentials());
        builder.RegisterService(&service);

        grpc::reflection::InitProtoReflectionServerBuilderPlugin();
        std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
        SPDLOG_INFO("🛰️ Config gRPC server listening on {}", address);
        server->Wait();
    } catch (const std::exception& e) {
        SPDLOG_ERROR("Failed to start gRPC server: {}", e.what());
    }
}
