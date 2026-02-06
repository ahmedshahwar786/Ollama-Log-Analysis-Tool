#ifndef OLLAMA_CLIENT_H
#define OLLAMA_CLIENT_H

#include <string>
#include "Config.h"

// Thin wrapper around the OLLAMA HTTP API.
// Uses libcurl .
class OllamaClient {
public:
    explicit OllamaClient(const AppConfig& cfg);

    // this will send a prompt to the local LLM and get back a plain text answer.
    // And returns empty string on failure.
    std::string sendChat(const std::string& prompt);

private:
    AppConfig config;
};

#endif 
