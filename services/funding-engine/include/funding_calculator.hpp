#ifndef FUNDING_CALCULATOR_HPP
#define FUNDING_CALCULATOR_HPP

#include <string>
#include <unordered_map>
#include <mutex>

struct FundingRate {
    double rate = 0.0;
    double longOi = 0.0;
    double shortOi = 0.0;
    std::string timestamp;
};

class FundingCalculator {
public:
    FundingCalculator(double multiplier = 0.01);

    void updateOpenInterest(const std::string& symbol, double longOi, double shortOi);
    FundingRate computeFunding(const std::string& symbol);

private:
    std::mutex mutex_;
    double k_;
    std::unordered_map<std::string, std::pair<double, double>> oiMap_;
};

#endif // FUNDING_CALCULATOR_HPP
