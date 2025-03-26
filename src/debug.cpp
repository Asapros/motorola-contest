#include "debug.hpp"
#include <iostream>

std::map<std::string, std::string> debugValues = std::map<std::string, std::string>();

void debugLog(std::string group, std::string message) {
    if (ENABLED_GROUPS.count(group) == 0) return;
    std::cout << "# DEBUG: " << group << ": " << message << std::endl;
}