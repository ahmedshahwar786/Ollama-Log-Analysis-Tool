#include "Summarizer.h"
#include "Utils.h"
#include <sstream>
#include <unordered_map>

//  "analysis" layer so we don't just dump raw logs to make the prompt small & useful.
std::string Summarizer::buildPrompt(const std::vector<LogEntry>& logs) {
    std::unordered_map<std::string, int> severityCounts;
    std::unordered_map<std::string, int> sourceCounts;
    int failedLogins = 0;
    int suspiciousWords = 0;

    for (const auto& log : logs) {
        std::string sev = Utils::toLower(log.severity);
        severityCounts[sev]++;

        sourceCounts[log.source]++;

        std::string msgLower = Utils::toLower(log.message);
        if (msgLower.find("failed password") != std::string::npos ||
            msgLower.find("login failed") != std::string::npos) {
            ++failedLogins;
        }

        if (msgLower.find("sql injection") != std::string::npos ||
            msgLower.find("xss") != std::string::npos ||
            msgLower.find("malware") != std::string::npos) {
            ++suspiciousWords;
        }
    }

    std::ostringstream oss;
    oss << "You are a SOC analyst assistant.\n";
    oss << "You are given a summarized view of recent log events.\n";
    oss << "Based on this, provide:\n";
    oss << "1) A high-level summary in 2–3 paragraphs.\n";
    oss << "2) The top 3 things an analyst should investigate first.\n";
    oss << "3) Any likely attack patterns (reference MITRE ATT&CK if appropriate).\n\n";

    oss << "=== Aggregate Stats ===\n";
    oss << "Total logs considered: " << logs.size() << "\n";

    oss << "Severity counts:\n";
    for (const auto& kv : severityCounts) {
        oss << "  " << kv.first << ": " << kv.second << "\n";
    }

    oss << "Top sources:\n";
    for (const auto& kv : sourceCounts) {
        oss << "  " << kv.first << ": " << kv.second << " events\n";
    }

    oss << "Failed login attempts detected (approx): " << failedLogins << "\n";
    oss << "Suspicious keyword hits (sql injection/xss/malware): " << suspiciousWords << "\n\n";

    oss << "=== Sample log snippets (redacted) ===\n";
    // It include first few messages so the model sees concrete examples
    int maxSnippets = 10;
    int count = 0;
    for (const auto& log : logs) {
        if (count >= maxSnippets) break;
        oss << "[" << log.timestamp << "] "
            << log.host << " "
            << log.severity << " "
            << log.source << " - "
            << log.message << "\n";
        ++count;
    }

    return oss.str();
}
