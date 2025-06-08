#include "report_generator.hpp"
#include "csv_writer.hpp"
#include "pdf_writer.hpp"
#include <filesystem>
#include <spdlog/spdlog.h>

namespace fs = std::filesystem;

ReportGenerator::ReportGenerator(const std::string& outputDir)
    : outputDir_(outputDir) {
    fs::create_directories(outputDir_);
}

bool ReportGenerator::generateCSVReport(const std::string& userId,
                                        const nlohmann::json& data,
                                        const std::string& type) {
    std::string filename = outputDir_ + "/" + userId + "_" + type + ".csv";
    return CSVWriter::write(filename, data);
}

bool ReportGenerator::generatePDFReport(const std::string& userId,
                                        const nlohmann::json& data,
                                        const std::string& type) {
    std::string filename = outputDir_ + "/" + userId + "_" + type + ".pdf";
    return PDFWriter::write(filename, data);
}
