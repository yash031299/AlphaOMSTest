#include "tick_parser.hpp"
#include <spdlog/spdlog.h>
#include <hiredis/hiredis.h>

void TickParser::process(redisReply* reply) {
    // This is a simplified placeholder
    SPDLOG_INFO("TickParser: parsing Redis reply (stream payload)");
    // In a real implementation, you would extract fields like price/qty/timestamp
}
