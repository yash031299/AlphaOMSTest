#pragma once

#include "order.pb.h"

class ExecutionRouter {
public:
    static order::OrderResponse route(const order::OrderRequest& req);
    
};
