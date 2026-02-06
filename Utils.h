#ifndef UTILS_H
#define UTILS_H

#include <string>

namespace Utils {

    // trim whitespace from both ends of a string.
    std::string trim(const std::string& s);

    // Basic check to avoid obviously dangerous file names.
    bool isSafePath(const std::string& path);

    // Simple lowercase helper for keyword checks.
    std::string toLower(const std::string& s);

}

#endif 
