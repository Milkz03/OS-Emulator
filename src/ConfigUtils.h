#pragma once

#include <string>

class ConfigUtils {
public:
    static bool isPowerOfTwo(unsigned int x);
    static bool isInValidRange(unsigned int x);
    static std::string stripQuotes(const std::string& str);
};

