#include <iostream>
#include <string>
#include <vector>
#include "Config.h"
#include "LogParser.h"
#include "OllamaClient.h"
#include "Summarizer.h"

//  CLI argument parsing.
struct CliOptions {
    std::string inputFile;
    std::string modelNameOverride;
    bool showHelp = false;
};

static void printUsage(const char* exeName) {
    std::cout <<
        "LogSense - SOC Log Summarizer using OLLAMA\n\n"
        "Usage:\n"
        "  " << exeName << " --input <path/to/logs.csv> [--model <ollama_model_name>]\n\n"
        "Example:\n"
        "  " << exeName << " --input samples/sample_logs.csv --model llama3\n";
}

static CliOptions parseArgs(int argc, char* argv[]) {
    CliOptions opts;
    if (argc <= 1) {
        opts.showHelp = true;
        return opts;
    }

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--help" || arg == "-h") {
            opts.showHelp = true;
            break;
        } else if (arg == "--input" && i + 1 < argc) {
            opts.inputFile = argv[++i];
        } else if (arg == "--model" && i + 1 < argc) {
            opts.modelNameOverride = argv[++i];
        } else {
            std::cerr << "[WARN] Unknown argument: " << arg << "\n";
        }
    }

    return opts;
}

int main(int argc, char* argv[]) {
    CliOptions options = parseArgs(argc, argv);
    if (options.showHelp) {
        printUsage(argv[0]);
        return 0;
    }

    if (options.inputFile.empty()) {
        std::cerr << "[ERROR] Missing required --input argument.\n";
        printUsage(argv[0]);
        return 1;
    }

    AppConfig config;
    if (!options.modelNameOverride.empty()) {
        config.modelName = options.modelNameOverride;
    }

    std::cout << "[INFO] Parsing log file: " << options.inputFile << "\n";
    auto logs = LogParser::parseCsv(options.inputFile, config);
    if (logs.empty()) {
        std::cerr << "[ERROR] No logs parsed. Exiting.\n";
        return 1;
    }

    std::cout << "[INFO] Building LLM prompt from " << logs.size() << " log entries...\n";
    std::string prompt = Summarizer::buildPrompt(logs);

    std::cout << "[INFO] Contacting OLLAMA model: " << config.modelName << "\n";
    OllamaClient client(config);
    std::string summary = client.sendChat(prompt);

    if (summary.empty()) {
        std::cerr << "[ERROR] LLM returned empty response.\n";
        return 1;
    }

    std::cout << "\n===== LLM SUMMARY =====\n";
    std::cout << summary << "\n";
    std::cout << "=======================\n";

    return 0;
}
