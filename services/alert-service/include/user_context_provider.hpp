#pragma once
#include "rule_engine.hpp"
#include <vector>

class UserContextProvider {
public:
    virtual std::vector<AlertContext> fetchAllContexts() = 0;
    virtual ~UserContextProvider() = default;
};
