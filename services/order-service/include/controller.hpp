#include "grpc_client.hpp"

class OrderController final : public order::OrderService::Service {
public:
    OrderController(std::shared_ptr<Validator> validator,
                    std::shared_ptr<ExecutionClient> exec_client);

    grpc::Status RouteOrder(grpc::ServerContext*, const order::OrderRequest*, order::OrderResponse*) override;

private:
    std::shared_ptr<Validator> validator_;
    std::shared_ptr<ExecutionClient> exec_client_;
};
