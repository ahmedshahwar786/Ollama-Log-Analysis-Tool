#ifndef LOG_PARSER_H
#define LOG_PARSER_H

#include <string>
#include <vector>
#include "LogEntry.h"
#include "Config.h"

class LogParser {
public:
    //  to parse a CSV log file into a vector of LogEntry objects.
    // only reads up to config.maxLogsToSend lines.
    static std::vector<LogEntry> parseCsv(const std::string& path, const AppConfig& config);
};

#endif 
