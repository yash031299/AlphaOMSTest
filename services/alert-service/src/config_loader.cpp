#include <cstdlib>
#include <fstream>
#include <stdexcept>
#include <string>
#include "config_loader.hpp"

std::string getEnvVar(const std::string& key, const std::string& fallback) {
    const char* val = std::getenv(key.c_str());
    if (val) return std::string(val);
    if (!fallback.empty()) return fallback;
    throw std::runtime_error("Missing env var: " + key);
}
