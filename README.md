
# Log Analysis Tool using OLLAMA (C++) 
## Overview
A command-line log analysis tool written in C++ that reads system/application logs from a CSV file, summarizes security-relevant activity, and generates an analyst-oriented report using a locally hosted LLM (Ollama). This project simulates a modern SOC workflow where automation assists analysts by identifying suspicious patterns, failed logins, and potential attack activity.

The tool demonstrates secure coding practices, modular design, and practical cybersecurity automation.


## Features
- CSV log parsing (timestamp, host, severity, source, message)
- Security-focused prompt generation from log entries
- Local LLM summarization via Ollama HTTP API (e.g., llama3)
- Detection of suspicious patterns:
  - Failed login attempts
  - Repeated admin requests
  - SQL injection indicators
- Modular and extendable C++ code structure



## Requirements

### Software
- Windows (MSYS2 / UCRT64)
- Ollama installed locally
- C++ compiler (g++, clang++)
- Git (optional)

### Dependencies
- C++ Standard Library (STL)
- libcurl (MSYS2):
  pacman -S mingw-w64-ucrt-x86_64-curl


## Ollama Model Setup
ollama pull llama3:latest


## Build Instructions
From UCRT64 shell, inside the project folder:

g++ main.cpp LogParser.cpp Summarizer.cpp OllamaClient.cpp Utils.cpp -o tool.exe -lcurl -lssl -lcrypto -lz -lws2_32 -lbcrypt


## Run
Start Ollama:
ollama serve

Run the tool:
./tool.exe --input sample_logs.csv --model llama3:latest


## Sample Input (sample_logs.csv)
Timestamp,Host,Severity,Source,Message
2025-11-26T10:01:22Z,web01,INFO,nginx,GET /index.html 200
2025-11-26T10:03:41Z,web01,WARN,nginx,Multiple 404 responses detected from IP 10.0.0.5
2025-11-26T10:05:12Z,auth01,ERROR,sshd,failed password for invalid user admin from 192.168.1.50 port 54321
2025-11-26T10:05:13Z,auth01,ERROR,sshd,failed password for invalid user admin from 192.168.1.50 port 54322
2025-11-26T10:05:14Z,auth01,ERROR,sshd,failed password for invalid user admin from 192.168.1.50 port 54323
2025-11-26T10:10:00Z,fw01,CRITICAL,firewall,Possible SQL injection attempt detected in HTTP request from 10.0.0.5
2025-11-26T10:15:30Z,web01,INFO,nginx,GET /login 200
2025-11-26T10:17:42Z,web01,WARN,nginx,repeated requests to /admin from unauthenticated session

---

## Sample Output (LLM Summary)

High-Level Summary:
The recent log events indicate a mix of benign and potentially malicious activity. Most logs relate to the Nginx web server, while SSH authentication errors and firewall alerts suggest potential attack activity.

Top 3 Things an Analyst Should Investigate:
1. Failed SSH login attempts for invalid user admin
2. Possible SQL injection attempt detected by firewall
3. Repeated unauthenticated requests to /admin

Likely Attack Patterns (MITRE ATT&CK):
- T1110 – Brute Force
- T1190 – Exploit Public-Facing Application
- T1595 – Active Scanning / Reconnaissance

---

## Testing
1. Run with provided sample_logs.csv
2. Confirm logs are parsed correctly
3. Confirm Ollama connection succeeds
4. Confirm summary output is generated

---

## Security Considerations
- Uses locally hosted LLM (no cloud data exposure)
- Input validation and error handling implemented
- No offensive or exploitation features
- Designed strictly for defensive security analysis

---

## Limitations
- LLM output quality depends on log quality and prompt design
- Large datasets may require batching
- Requires Ollama running locally

## Screenshot
<img width="1443" height="343" alt="Output" src="https://github.com/user-attachments/assets/58da998d-7f6c-49be-870b-74a48bdda349" />


## Disclaimer
Educational and defensive cybersecurity project only.
