#pragma once
struct redisReply;

class TickParser {
public:
    void process(redisReply* reply);
};
