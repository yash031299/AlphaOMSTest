#pragma once
#include <string>
#include <unordered_map>
#include <mutex>
#include <nlohmann/json.hpp>

class ConfigLoader {
public:
    static void loadEnv(const std::string& path = ".env");
    static std::string getEnv(const std::string& key);

    static bool loadConfig(const std::string& jsonPath);
    static nlohmann::json getSymbolConfig(const std::string& symbol);
    static std::unordered_map<std::string, std::string> getAllEnv();
    static bool reloadConfig();

private:
    static std::unordered_map<std::string, std::string> envConfig;
    static nlohmann::json symbolConfigs;
    static std::mutex configMutex;
};
