#pragma once

#include "controller.hpp"

class StraightGuy : public Controller {
    Controls computeControls(World* world, EntityId playerId);
};
