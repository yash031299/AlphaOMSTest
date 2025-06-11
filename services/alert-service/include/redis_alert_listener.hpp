#pragma once
#include <memory>
class AlertDispatcher;

class RedisAlertListener {
public:
    void start(std::shared_ptr<AlertDispatcher> dispatcher);
};
