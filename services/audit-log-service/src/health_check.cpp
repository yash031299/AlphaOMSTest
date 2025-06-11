#include "health_check.hpp"
#include <grpcpp/ext/proto_server_reflection_plugin.h>

void HealthCheckServer::enable(grpc::ServerBuilder& builder) {
    // Enable standard health check service
    grpc::EnableDefaultHealthCheckService(true);

    // Optional: also enable reflection for grpcurl testing
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();
}
