#pragma once

#include "common.hpp"
#include <vector>

struct PlayerInfo {
    EntityId playerId;
    unsigned short place;
    double seconds;
    unsigned int checkpoint;
};