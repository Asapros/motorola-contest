#include "controller.hpp"
#include <raylib.h>

PlayerController::PlayerController()
    : key_forward(KEY_W),
      key_backward(KEY_S),
      key_right(KEY_D),
      key_left(KEY_A) {}

Controls PlayerController::computeControls() {
    float accelerator = 0.0;
    if (IsKeyDown(key_forward)) {
        accelerator = 1.0;
    } else if (IsKeyDown(key_backward)) {
        accelerator = -1.0;
    }
    float steering = 0.0;
    if (IsKeyDown(key_right)) {
        steering = 1.0;
    } else if (IsKeyDown(key_left)) {
        steering = -1.0;
    }
    return Controls{accelerator, steering};
}
