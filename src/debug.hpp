#pragma once

#include <string>
#include <set>
#include <map>

static const std::set<std::string> ENABLED_GROUPS = {
    "MODEL",
    "MENU",
    "GENERAL",
    "CONTROL"
};

extern std::map<std::string, std::string> debugValues;

void debugLog(std::string group, std::string message);