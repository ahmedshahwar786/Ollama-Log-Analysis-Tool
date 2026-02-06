#include "OllamaClient.h"
#include <curl/curl.h>
#include <iostream>
#include <sstream>

// ----------------------------------------
// JSON ESCAPE FUNCTION (FIXES HTTP 400)
// ----------------------------------------
static std::string escapeJson(const std::string& input) {
    std::ostringstream oss;
    for (char c : input) {
        switch (c) {
            case '\\': oss << "\\\\"; break;
            case '"':  oss << "\\\""; break;
            case '\n': oss << "\\n"; break;
            case '\r': oss << "\\r"; break;
            case '\t': oss << "\\t"; break;
            default:   oss << c;     break;
        }
    }
    return oss.str();
}

// ----------------------------------------
// CURL write callback
// ----------------------------------------
static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t total = size * nmemb;
    std::string* buffer = static_cast<std::string*>(userp);
    buffer->append((char*)contents, total);
    return total;
}

// Extract `"response":"..."`
static std::string extractContentField(const std::string& json) {
    const std::string key = "\"response\":\"";
    size_t pos = json.find(key);
    if (pos == std::string::npos) return "";

    pos += key.size();
    bool escape = false;
    std::string out;

    for (size_t i = pos; i < json.size(); i++) {
        char c = json[i];

        if (escape) {
            out.push_back(c);
            escape = false;
        }
        else if (c == '\\') {
            escape = true;
        }
        else if (c == '"') {
            break;
        }
        else {
            out.push_back(c);
        }
    }
    return out;
}

OllamaClient::OllamaClient(const AppConfig& cfg) : config(cfg) {}

std::string OllamaClient::sendChat(const std::string& prompt) {

    // Escape logs → valid JSON
    std::string safePrompt = escapeJson(prompt);

    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "[ERROR] Failed to init CURL\n";
        return "";
    }

    std::string responseBody;

    // --------- CORRECT JSON BODY FOR /api/generate ----------
    std::string jsonBody =
        "{"
        "\"model\":\"" + config.modelName + "\","
        "\"prompt\":\"" + safePrompt + "\","
        "\"stream\":false"
        "}";

    curl_easy_setopt(curl, CURLOPT_URL, config.ollamaUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonBody.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, jsonBody.size());

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBody);

    // Timeouts
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, config.connectTimeoutSeconds);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, config.requestTimeoutSeconds);

    // Headers
    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    CURLcode res = curl_easy_perform(curl);

    long httpCode = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        std::cerr << "[ERROR] CURL failed: "
                  << curl_easy_strerror(res) << std::endl;
        return "";
    }

    if (httpCode != 200) {
        std::cerr << "[ERROR] OLLAMA returned HTTP " << httpCode << std::endl;
        std::cerr << "[DEBUG] Raw Response: " << responseBody << std::endl;
        return "";
    }

    return extractContentField(responseBody);
}
