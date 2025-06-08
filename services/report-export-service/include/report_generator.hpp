#ifndef REPORT_GENERATOR_HPP
#define REPORT_GENERATOR_HPP

#include <string>
#include <nlohmann/json.hpp>

class ReportGenerator {
public:
    explicit ReportGenerator(const std::string& outputDir);

    bool generateCSVReport(const std::string& userId,
                           const nlohmann::json& data,
                           const std::string& type);

    bool generatePDFReport(const std::string& userId,
                           const nlohmann::json& data,
                           const std::string& type);

private:
    std::string outputDir_;
};

#endif // REPORT_GENERATOR_HPP
