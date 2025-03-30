#pragma once

#include "world.hpp"

#include <raylib.h>

const int MIN_GEAR = 1;
const int MAX_GEAR = 5;

struct Controls {
    // Both those fields should be in range [-1, 1]
    float accelerator;
    float steering;

    int gear;
};

class Controller {
   public:
    virtual ~Controller();
    virtual Controls computeControls(World* world, EntityId playerId) = 0;
};

class PlayerController : public Controller {
   public:
    KeyboardKey key_forward;
    KeyboardKey key_backward;
    KeyboardKey key_right;
    KeyboardKey key_left;
    KeyboardKey key_lower_gear;
    KeyboardKey key_rise_gear;

    PlayerController();
    Controls computeControls(World* world, EntityId playerId);
};
