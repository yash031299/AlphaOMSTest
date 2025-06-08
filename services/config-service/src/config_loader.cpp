#include "config_loader.hpp"
#include <fstream>
#include <sstream>
#include <spdlog/spdlog.h>

std::unordered_map<std::string, std::string> ConfigLoader::config;

void ConfigLoader::load(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        SPDLOG_ERROR("Failed to open .env file: {}", path);
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        size_t eq = line.find('=');
        if (eq != std::string::npos) {
            std::string key = line.substr(0, eq);
            std::string val = line.substr(eq + 1);
            config[key] = val;
        }
    }
    SPDLOG_INFO(".env loaded from {}", path);
}

std::string ConfigLoader::get(const std::string& key) {
    return config.count(key) ? config[key] : "";
}
