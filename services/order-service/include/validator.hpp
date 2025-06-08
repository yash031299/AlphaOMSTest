#pragma once

#include <memory>
#include "symbol_client.hpp"
#include "user_client.hpp"
#include "order.pb.h"

class Validator {
public:
    Validator(std::shared_ptr<SymbolClient> symbol_client,
              std::shared_ptr<UserClient> user_client);

    bool validateOrder(const order::OrderRequest &order);

private:
    std::shared_ptr<SymbolClient> symbol_client;
    std::shared_ptr<UserClient> user_client;
};
