#include "funding_publisher.hpp"
#include <spdlog/spdlog.h>

void FundingPublisher::publish(const std::string& symbol, const FundingRate& rate) {
    SPDLOG_INFO("[FUNDING] {} @ {} | Rate: {:.6f} | Long: {:.2f} | Short: {:.2f}",
                symbol, rate.timestamp, rate.rate, rate.longOi, rate.shortOi);
}
