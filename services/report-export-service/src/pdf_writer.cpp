#include "pdf_writer.hpp"
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <spdlog/spdlog.h>

static std::string buildHTML(const nlohmann::json& data) {
    std::stringstream html;
    html << "<html><head><style>"
         << "body{font-family:sans-serif;padding:20px}"
         << "table{border-collapse:collapse;width:100%}"
         << "th,td{border:1px solid #ccc;padding:8px;text-align:left}"
         << "</style></head><body>"
         << "<h2>User Report</h2><table><thead><tr>";

    if (data.empty() || !data.is_array()) return "Invalid data";

    const auto& first = data[0];
    for (auto it = first.begin(); it != first.end(); ++it)
        html << "<th>" << it.key() << "</th>";
    html << "</tr></thead><tbody>";

    for (const auto& obj : data) {
        html << "<tr>";
        for (auto it = first.begin(); it != first.end(); ++it)
            html << "<td>" << obj.value(it.key(), "") << "</td>";
        html << "</tr>";
    }

    html << "</tbody></table></body></html>";
    return html.str();
}

bool PDFWriter::write(const std::string& outputPath, const nlohmann::json& data) {
    std::string tempHtml = outputPath + ".html";
    std::ofstream file(tempHtml);
    if (!file.is_open()) {
        SPDLOG_ERROR("PDFWriter: Cannot write temporary HTML file.");
        return false;
    }

    file << buildHTML(data);
    file.close();

    std::string cmd = "wkhtmltopdf " + tempHtml + " " + outputPath;
    int status = std::system(cmd.c_str());
    if (status != 0) {
        SPDLOG_ERROR("PDFWriter: wkhtmltopdf failed with status {}", status);
        return false;
    }

    std::remove(tempHtml.c_str());
    SPDLOG_INFO("PDF report written to {}", outputPath);
    return true;
}
