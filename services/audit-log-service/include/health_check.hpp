#pragma once
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

class HealthCheckServer {
public:
    static void enable(grpc::ServerBuilder& builder);
};
