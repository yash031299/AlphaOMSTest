#ifndef PDF_WRITER_HPP
#define PDF_WRITER_HPP

#include <string>
#include <nlohmann/json.hpp>

class PDFWriter {
public:
    static bool write(const std::string& outputPath, const nlohmann::json& data);
};

#endif // PDF_WRITER_HPP
