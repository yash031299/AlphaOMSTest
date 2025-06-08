#pragma once
#include <string>
#include "router.pb.h"

class OrderRouter {
public:
    OrderRouter();
    router::OrderResponse route(const router::OrderRequest& request);
};
