#ifndef FUNDING_PUBLISHER_HPP
#define FUNDING_PUBLISHER_HPP

#include "funding_calculator.hpp"
#include <string>

class FundingPublisher {
public:
    void publish(const std::string& symbol, const FundingRate& rate);
};

#endif // FUNDING_PUBLISHER_HPP
