#include "log_writer.hpp"
#include <nlohmann/json.hpp>
#include <chrono>
#include <iomanip>
#include <filesystem>


namespace fs = std::filesystem;
using json = nlohmann::json;

LogWriter::LogWriter(const std::string& baseDir) : baseDir_(baseDir) {
    fs::create_directories(baseDir + "/trade");
    fs::create_directories(baseDir + "/order");
    fs::create_directories(baseDir + "/funding");
    fs::create_directories(baseDir + "/liquidation");
}

std::string LogWriter::todayDate() {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&t), "%F");
    return ss.str(); // e.g., "2025-06-07"
}

void LogWriter::appendToFile(const std::string& path, const std::string& jsonStr) {
    std::lock_guard<std::mutex> lock(mutex_);
    std::ofstream ofs(path, std::ios::app);
    ofs << jsonStr << "\n";
}

void LogWriter::writeTrade(const TradeEvent& e) {
    appendToFile(baseDir_ + "/trade/" + todayDate() + ".log", toJson(e));
}

void LogWriter::writeOrder(const OrderEvent& e) {
    appendToFile(baseDir_ + "/order/" + todayDate() + ".log", toJson(e));
}

void LogWriter::writeFunding(const FundingEvent& e) {
    appendToFile(baseDir_ + "/funding/" + todayDate() + ".log", toJson(e));
}

void LogWriter::writeLiquidation(const LiquidationEvent& e) {
    appendToFile(baseDir_ + "/liquidation/" + todayDate() + ".log", toJson(e));
}

std::string LogWriter::toJson(const TradeEvent& e) {
    return json{
        {"type", "trade"},
        {"symbol", e.symbol()},
        {"maker", e.makerorderid()},
        {"taker", e.takerorderid()},
        {"price", e.price()},
        {"quantity", e.quantity()},
        {"timestamp", e.timestamp()}
    }.dump();
}

std::string LogWriter::toJson(const OrderEvent& e) {
    return json{
        {"type", "order"},
        {"orderId", e.orderid()},
        {"userId", e.userid()},
        {"symbol", e.symbol()},
        {"side", e.side()},
        {"type_detail", e.type_detail()},
        {"price", e.price()},
        {"quantity", e.quantity()},
        {"status", e.status()},
        {"timestamp", e.timestamp()}
    }.dump();
}

std::string LogWriter::toJson(const FundingEvent& e) {
    return json{
        {"type", "funding"},
        {"symbol", e.symbol()},
        {"rate", e.fundingrate()},
        {"longOi", e.longoi()},
        {"shortOi", e.shortoi()},
        {"timestamp", e.timestamp()}
    }.dump();
}

std::string LogWriter::toJson(const LiquidationEvent& e) {
    return json{
        {"type", "liquidation"},
        {"userId", e.userid()},
        {"symbol", e.symbol()},
        {"pnl", e.pnl()},
        {"reason", e.reason()},
        {"timestamp", e.timestamp()}
    }.dump();
}
