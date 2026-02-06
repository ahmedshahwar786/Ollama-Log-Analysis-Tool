#include "Utils.h"
#include <algorithm>
#include <cctype>

namespace Utils {

    std::string trim(const std::string& s) {
        if (s.empty()) return s;
        size_t start = 0;
        while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))) {
            ++start;
        }
        if (start == s.size()) return "";

        size_t end = s.size() - 1;
        while (end > start && std::isspace(static_cast<unsigned char>(s[end]))) {
            --end;
        }
        return s.substr(start, end - start + 1);
    }

    bool isSafePath(const std::string& path) {
        //  for sanity checks :
        // no null bytes
        //  no obvious traversal like "../"
        //  not empty
        if (path.empty()) return false;
        if (path.find('\0') != std::string::npos) return false;
        if (path.find("..") != std::string::npos) return false;
        return true;
    }

    std::string toLower(const std::string& s) {
        std::string out = s;
        std::transform(out.begin(), out.end(), out.begin(),
                       [](unsigned char c){ return static_cast<char>(std::tolower(c)); });
        return out;
    }

}
