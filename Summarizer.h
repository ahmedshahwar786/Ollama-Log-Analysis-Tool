#ifndef SUMMARIZER_H
#define SUMMARIZER_H

#include <string>
#include <vector>
#include "LogEntry.h"

//  for turning raw parsed logs into a single prompt string to send LLM.
class Summarizer {
public:
    static std::string buildPrompt(const std::vector<LogEntry>& logs);
};

#endif 
