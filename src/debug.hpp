#pragma once

#include <string>
#include <set>

static const std::set<std::string> ENABLED_GROUPS = {
    "MODEL",
    "MENU",
};

void debugLog(std::string group, std::string message);