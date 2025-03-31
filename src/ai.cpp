#include "ai.hpp"
#include <cmath>
#include "common.hpp"
#include "debug.hpp"
#include "raylib.h"
#include "vehicle.hpp"
#include "world.hpp"

Controls followPoint(Vector2 point, Vector2 position, float orientation) {
    float accelerator = 0.2;
    float steering = 0.0;

    float dx = point.x - position.x;
    float dy = point.y - position.y;

    float desiredAngle = -atan2(dy, dx);

    debugValues["ai_y_del"] = std::to_string(dy);
    debugValues["ai_x_del"] = std::to_string(dx);

    steering = (desiredAngle - orientation) * 0.5;
    debugValues["ai_desired_angle"] = std::to_string(desiredAngle);
    return Controls{accelerator, steering, 5};
}

Controls StraightGuy::computeControls(World* world, EntityId aiId) {
    auto player = dynamic_cast<Vehicle*>(world->entities[aiId].get());
    return followPoint({0, 0}, Vector2{player->position.x, player->position.z},
                       player->heading);
}
