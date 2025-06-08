#include "funding_calculator.hpp"
#include <chrono>
#include <iomanip>
#include <sstream>

FundingCalculator::FundingCalculator(double multiplier)
    : k_(multiplier) {}

void FundingCalculator::updateOpenInterest(const std::string& symbol, double longOi, double shortOi) {
    std::lock_guard<std::mutex> lock(mutex_);
    oiMap_[symbol] = {longOi, shortOi};
}

FundingRate FundingCalculator::computeFunding(const std::string& symbol) {
    std::lock_guard<std::mutex> lock(mutex_);

    double rate = 0.0, longOi = 0.0, shortOi = 0.0;
    if (oiMap_.count(symbol)) {
        std::tie(longOi, shortOi) = oiMap_[symbol];
        double totalOi = longOi + shortOi;
        if (totalOi > 0) {
            rate = (longOi - shortOi) / totalOi * k_;
        }
    }

    auto now = std::chrono::system_clock::now();
    std::time_t ts = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&ts), "%F %T");

    return FundingRate{
        .rate = rate,
        .longOi = longOi,
        .shortOi = shortOi,
        .timestamp = ss.str()
    };
}
