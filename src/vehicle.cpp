#include <cmath>

#include <raylib.h>
#include <raymath.h>

#include "common.hpp"
#include "rigidbody.hpp"
#include "vehicle.hpp"

// Backported from https://github.com/raysan5/raylib/pull/4520 - there was no
// stable release since that pull request
float PortedVector2CrossProduct(Vector2 v1, Vector2 v2) {
    float result = (v1.x * v2.y - v1.y * v2.x);

    return result;
}

void Vehicle::update(float delta_time) {
    Rigidbody::update(delta_time);

    heading += angular_momentum / moment_of_intertia;

    Vector3 velocity = computeVelocity();

    float grav_force = mass * GRAVITY_ACCELERATION;

    float torque = 0.0;

    for (auto& wheel : wheels) {
        // Velocity of the bottom point on the wheel
        float bottom_velocity_rel = wheel.angular_velocity * wheel.radius;

        float wheel_heading = heading + wheel.steering;
        Vector2 wheel_heading_vec{(float)cos(wheel_heading),
                                  (float)sin(wheel_heading)};
        Vector2 bottom_velocity_rel_vec =
            Vector2Scale(wheel_heading_vec, bottom_velocity_rel);

        Vector2 delta_velocity =
            Vector2Subtract({velocity.x, velocity.y}, bottom_velocity_rel_vec);
        float delta_velocity_len = Vector2Length(delta_velocity);

        // TODO: determine from tire and ground material
        float static_friction_coef = 0.8;
        float kinetic_friction_coef = 0.4;

        float max_static_friction =
            static_friction_coef * grav_force / wheels.size();
        float kinetic_friction =
            kinetic_friction_coef * grav_force / wheels.size();

        Vector2 friction_dir = Vector2Normalize(delta_velocity);

        Vector2 wheel_position_rotated =
            Vector2Rotate(wheel.position_relative, heading);

        float cos_wheel_heading_friction_force =
            Vector2DotProduct(wheel_heading_vec, friction_dir);
        float cos_wheel_pos_friction_force =
            Vector2DotProduct(wheel_position_rotated, friction_dir);
        float sin_wheel_pos_friction_force =
            PortedVector2CrossProduct(wheel_position_rotated, friction_dir);

        float wheel_ang_acc_required_to_stop =
            delta_velocity_len / (wheel.radius * delta_time);
        float vehicle_acc_required_to_stop = delta_velocity_len / delta_time;
        float friction_required_to_stop =
            (wheel_ang_acc_required_to_stop * wheel.moment_of_intertia /
             (std::max<float>(cos_wheel_heading_friction_force, 1.0e9) *
              wheel.radius)) +
            (vehicle_acc_required_to_stop * mass /
             std::max<float>(cos_wheel_pos_friction_force, 1.0e9));

        float actual_friction;
        if (friction_required_to_stop <= max_static_friction) {
            actual_friction = friction_required_to_stop;
        } else {
            actual_friction = kinetic_friction;
        }

        // Apply forces and torques
        wheel.angular_velocity +=
            actual_friction * cos_wheel_heading_friction_force * wheel.radius *
            delta_time / wheel.moment_of_intertia;
        applyForce(Vector3Scale(Vector3{friction_dir.x, friction_dir.y, 0.0},
                                actual_friction),
                   delta_time);
        /*torque += */
    }
}
