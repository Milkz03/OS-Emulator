#include "ConfigUtils.h"

bool ConfigUtils::isPowerOfTwo(unsigned int x) {
    return x && !(x & (x - 1));
}

bool ConfigUtils::isInValidRange(unsigned int x) {
    return x >= 2 && x <= 0xFFFFFFFF;  // [2, 2^32]
}

std::string ConfigUtils::stripQuotes(const std::string& str) {
    if (str.size() >= 2 &&
        ((str.front() == '"' && str.back() == '"') ||
            (str.front() == '\'' && str.back() == '\''))) {
        return str.substr(1, str.size() - 2);
    }
    return str;
}