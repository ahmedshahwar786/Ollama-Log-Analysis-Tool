#ifndef CONFIG_H
#define CONFIG_H

#include <string>

// Simple config struct for the tool.
struct AppConfig {
    std::string ollamaUrl = "http://localhost:11434/api/generate";
    std::string modelName = "llama3";
    int maxLogsToSend = 200;         //  limit so we don't overload the model
    int connectTimeoutSeconds = 5;   // to avoid hanging forever
    int requestTimeoutSeconds = 120;  // for reasonable network timeout
};

#endif // CONFIG_H
