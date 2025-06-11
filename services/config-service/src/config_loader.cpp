#include "config_loader.hpp"
#include <fstream>
#include <sstream>
#include <spdlog/spdlog.h>

std::unordered_map<std::string, std::string> ConfigLoader::envConfig;
nlohmann::json ConfigLoader::symbolConfigs;
std::mutex ConfigLoader::configMutex;

void ConfigLoader::loadEnv(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        SPDLOG_ERROR("❌ Failed to open .env file: {}", path);
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        size_t eq = line.find('=');
        if (eq != std::string::npos) {
            std::string key = line.substr(0, eq);
            std::string val = line.substr(eq + 1);
            envConfig[key] = val;
        }
    }
    SPDLOG_INFO("✅ Loaded .env from {}", path);
}

std::string ConfigLoader::getEnv(const std::string& key) {
    return envConfig.count(key) ? envConfig[key] : "";
}

std::unordered_map<std::string, std::string> ConfigLoader::getAllEnv() {
    return envConfig;
}

bool ConfigLoader::loadConfig(const std::string& jsonPath) {
    try {
        std::ifstream in(jsonPath);
        if (!in) {
            SPDLOG_ERROR("❌ Config JSON not found at path: {}", jsonPath);
            return false;
        }

        in >> symbolConfigs;
        SPDLOG_INFO("✅ Loaded symbol config from {}", jsonPath);
        return true;
    } catch (const std::exception& e) {
        SPDLOG_ERROR("❌ Failed to parse config JSON: {}", e.what());
        return false;
    }
}

nlohmann::json ConfigLoader::getSymbolConfig(const std::string& symbol) {
    std::lock_guard<std::mutex> lock(configMutex);
    if (symbolConfigs.contains(symbol)) {
        return symbolConfigs[symbol];
    }
    return {}; // return empty object if not found
}

bool ConfigLoader::reloadConfig() {
    std::lock_guard<std::mutex> lock(configMutex);
    std::string path = getEnv("CONFIG_JSON_PATH");
    if (path.empty()) {
        SPDLOG_ERROR("❌ CONFIG_JSON_PATH missing in .env");
        return false;
    }
    return loadConfig(path);
}