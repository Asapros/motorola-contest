#include "debug.hpp"
#include <iostream>

void debugLog(std::string group, std::string message) {
    if (ENABLED_GROUPS.count(group) == 0) return;
    std::cout << "# DEBUG: " << group << ": " << message << std::endl;
}