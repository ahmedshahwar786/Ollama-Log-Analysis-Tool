#ifndef LOG_ENTRY_H
#define LOG_ENTRY_H

#include <string>

// It will represents a single parsed log line from the CSV file.
struct LogEntry {
    std::string timestamp;
    std::string host;
    std::string severity; // like INFO, WARN, ERROR, CRITICAL
    std::string source;   // like sshd, nginx, firewall
    std::string message;  // raw log message text
};

#endif 
