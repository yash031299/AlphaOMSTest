#include "csv_writer.hpp"
#include <fstream>
#include <sstream>
#include <spdlog/spdlog.h>

bool CSVWriter::write(const std::string& filename, const nlohmann::json& data) {
    if (!data.is_array() || data.empty()) {
        SPDLOG_ERROR("CSVWriter: Input data is not a valid JSON array.");
        return false;
    }

    std::ofstream file(filename);
    if (!file.is_open()) {
        SPDLOG_ERROR("CSVWriter: Could not open file {}", filename);
        return false;
    }

    // Write header
    const auto& first = data[0];
    bool firstField = true;
    for (auto it = first.begin(); it != first.end(); ++it) {
        if (!firstField) file << ",";
        file << it.key();
        firstField = false;
    }
    file << "\n";

    // Write rows
    for (const auto& obj : data) {
        firstField = true;
        for (auto it = first.begin(); it != first.end(); ++it) {
            if (!firstField) file << ",";
            file << obj.value(it.key(), "");
            firstField = false;
        }
        file << "\n";
    }

    file.close();
    SPDLOG_INFO("CSV report written to {}", filename);
    return true;
}
