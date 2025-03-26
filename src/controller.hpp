#pragma once

#include <raylib.h>

struct Controls {
    // Both those fields should be in range [-1, 1]
    float accelerator;
    float steering;
    // TODO: gears, etc.
};

class Controller {
   public:
    virtual Controls computeControls() = 0;
};

class PlayerController : public Controller {
   public:
    KeyboardKey key_forward;
    KeyboardKey key_backward;
    KeyboardKey key_right;
    KeyboardKey key_left;

    PlayerController();
    Controls computeControls();
};
