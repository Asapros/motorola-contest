#include "ai.hpp"
#include "raylib.h"
#include "world.hpp"
#include "common.hpp"

Controls StraightGuy::computeControls(World* world, EntityId aiId) {
    float accelerator = 1.0;
    float steering = 0.2;
    return Controls{accelerator, steering, 5};
}