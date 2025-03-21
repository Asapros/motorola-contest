#include "vehicle.hpp"
#include <raymath.h>
#include <cmath>
#include "rigidbody.hpp"

void Vehicle::update() {
    Rigidbody::update();

    heading += angular_momentum / moment_of_intertia;

    for (auto& wheel : wheels) {
        float bottom_velocity_rel = wheel.angular_velocity * wheel.radius;

        Vector3 velocity = computeVelocity();
        Vector2 heading_vec{cos(heading), sin(heading)};
        Vector2 bottom_velocity_rel_vec =
            Vector2Scale(heading_vec, bottom_velocity_rel);

        Vector2 delta_velocity =
            Vector2Subtract({velocity.x, velocity.y}, bottom_velocity_rel_vec);
    }
}
