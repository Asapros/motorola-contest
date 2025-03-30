#include "controller.hpp"
#include <raylib.h>
#include <algorithm>

#include <format>

#include "debug.hpp"
#include "vehicle.hpp"

Controller::~Controller() {}

PlayerController::PlayerController()
    : key_forward(KEY_W),
      key_backward(KEY_S),
      key_right(KEY_D),
      key_left(KEY_A),
      key_lower_gear(KEY_Q),
      key_rise_gear(KEY_E) {}

Controls PlayerController::computeControls(World* world, EntityId playerId) {
    auto player = dynamic_cast<Vehicle*>(world->entities[playerId].get());

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

    int gear = player->gear;
    if (IsKeyPressed(key_lower_gear)) gear--;
    else if (IsKeyPressed(key_rise_gear)) gear++;
    gear = std::max(std::min(MAX_GEAR, gear), MIN_GEAR); // cap

    return Controls{accelerator, steering, gear};
}
