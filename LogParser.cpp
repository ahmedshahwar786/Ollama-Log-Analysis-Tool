#include "LogParser.h"
#include "Utils.h"
#include <fstream>
#include <sstream>
#include <iostream>

//  CSV parser for this specific use case.

std::vector<LogEntry> LogParser::parseCsv(const std::string& path, const AppConfig& config) {
    std::vector<LogEntry> logs;

    if (!Utils::isSafePath(path)) {
        std::cerr << "[ERROR] Log path looks unsafe. Aborting: " << path << "\n";
        return logs;
    }

    std::ifstream in(path);
    if (!in.is_open()) {
        std::cerr << "[ERROR] Could not open log file: " << path << "\n";
        return logs;
    }

    std::string line;
    bool firstLine = true;
    int count = 0;

    while (std::getline(in, line)) {
        if (line.empty()) {
            continue;
        }

        // to skip header row
        if (firstLine) {
            firstLine = false;
            continue;
        }

        std::stringstream ss(line);
        std::string field;
        std::vector<std::string> fields;

        // to split by comma.
        while (std::getline(ss, field, ',')) {
            fields.push_back(Utils::trim(field));
        }

        if (fields.size() < 5) {
            // just log and continue instead of crashing.
            std::cerr << "[WARN] Skipping malformed line: " << line << "\n";
            continue;
        }

        LogEntry entry;
        entry.timestamp = fields[0];
        entry.host      = fields[1];
        entry.severity  = fields[2];
        entry.source    = fields[3];
        entry.message   = fields[4];

        logs.push_back(entry);
        ++count;

        if (count >= config.maxLogsToSend) {
            std::cerr << "[INFO] Reached max log limit (" << config.maxLogsToSend << "). Remaining lines ignored.\n";
            break;
        }
    }

    return logs;
}
