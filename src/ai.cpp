#include "ai.hpp"
#include "raylib.h"
#include "world.hpp"
#include "vehicle.hpp"
#include "common.hpp"
#include "debug.hpp"
#include <cmath>

Controls followPoint(Vector2 point, Vector2 position, float orientation) {
    float accelerator = 1;
    float steering = 0.0;

    float dx = position.x - point.x;
    float dy = position.y - point.y;

    float desiredAngle = atan2(dy, dx);

    debugValues["ai_y_del"] = std::to_string(position.y-point.y);
    debugValues["ai_x_del"] = std::to_string(position.x-point.x);

    steering = (desiredAngle - orientation);
    debugValues["ai_desired_angle"] = std::to_string(desiredAngle);
    return Controls{accelerator, steering, 5};
}

Controls StraightGuy::computeControls(World* world, EntityId aiId) {
    auto player = dynamic_cast<Vehicle*>(world->entities[aiId].get());
    return followPoint({0, 0}, Vector2{player->position.x, player->position.z}, player->heading);
}