#pragma once
#include <string>
#include <unordered_map>

class ConfigLoader {
public:
    static void load(const std::string& path = ".env");
    static std::string get(const std::string& key);
private:
    static std::unordered_map<std::string, std::string> config;
};
