#include "grpc_server.hpp"
#include "config_loader.hpp"
#include "config.grpc.pb.h"
#include <grpcpp/grpcpp.h>
#include <spdlog/spdlog.h>

class ConfigServiceImpl final : public config::ConfigService::Service {
public:
    grpc::Status GetConfig(grpc::ServerContext*,
                           const config::ConfigRequest* request,
                           config::ConfigValue* response) override {
        std::string val;
        bool found = ConfigLoader::get().get(request->key(), val);
        response->set_value(val);
        response->set_found(found);
        return grpc::Status::OK;
    }

    grpc::Status GetAll(grpc::ServerContext*,
                        const config::Empty*,
                        config::ConfigMap* response) override {
        auto map = ConfigLoader::get().getAll();
        (*response->mutable_configs()) = {map.begin(), map.end()};
        return grpc::Status::OK;
    }
};

void ConfigGRPCServer::run(const std::string& address) {
    ConfigServiceImpl service;
    grpc::ServerBuilder builder;
    builder.AddListeningPort(address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    SPDLOG_INFO("🛰️ Config gRPC server listening on {}", address);
    server->Wait();
}
