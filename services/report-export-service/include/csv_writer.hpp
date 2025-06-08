#ifndef CSV_WRITER_HPP
#define CSV_WRITER_HPP

#include <string>
#include <nlohmann/json.hpp>

class CSVWriter {
public:
    static bool write(const std::string& filename, const nlohmann::json& data);
};

#endif // CSV_WRITER_HPP
