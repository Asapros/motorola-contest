#include <cmath>
#include <iostream>

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

Vehicle::Vehicle(Model& model,
                 Vector3 position,
                 float mass,
                 float moment_of_intertia,
                 std::vector<Wheel> wheels)
    : Rigidbody(model, position, mass),
      angular_momentum(0.0),
      moment_of_intertia(moment_of_intertia),
      wheels(wheels) {}

void Vehicle::update(float delta_time) {
    Rigidbody::update(delta_time);

    // Angular momentum drag
    angular_momentum *= std::pow(0.85, delta_time);
    std::cerr << "angular_momentum = " << angular_momentum << '\n';

    heading += angular_momentum / moment_of_intertia;
    heading = fmod(heading, 2 * PI);
    std::cerr << "heading = " << heading << '\n';

    Vector3 velocity = computeVelocity();

    float grav_force = mass * GRAVITY_ACCELERATION;

    float torque = 0.0;
    Vector2 force = {0.0, 0.0};

    for (auto& wheel : wheels) {
        // Drag
        wheel.angular_velocity *= std::pow(0.95, delta_time);

        std::cerr << "angular_velocity = " << wheel.angular_velocity << '\n';
        std::cerr << "steering = " << wheel.steering << '\n';

        // Velocity of the bottom point on the wheel
        float bottom_velocity_rel = wheel.angular_velocity * wheel.radius;

        float wheel_heading = heading + wheel.steering;
        Vector2 wheel_heading_vec{(float)cos(wheel_heading),
                                  (float)sin(wheel_heading)};
        Vector2 bottom_velocity_rel_vec =
            Vector2Scale(wheel_heading_vec, bottom_velocity_rel);

        Vector2 velocity_with_rot = Vector2Add(
            Vector2{velocity.x, velocity.z},
            Vector2Scale(
                Vector2Rotate(Vector2Normalize(wheel.position_relative),
                              heading + (PI / 2.0)),
                angular_momentum * Vector2Length(wheel.position_relative) /
                    moment_of_intertia));

        Vector2 delta_velocity =
            Vector2Subtract(velocity_with_rot, bottom_velocity_rel_vec);
        float delta_velocity_len = Vector2Length(delta_velocity);

        std::cerr << "delta_velocity_len = " << delta_velocity_len << '\n';

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
        std::cerr << "cos_wheel_heading_friction_force = "
                  << cos_wheel_heading_friction_force << '\n';
        float cos_wheel_pos_friction_force = Vector2DotProduct(
            Vector2Normalize(wheel_position_rotated), friction_dir);
        std::cerr << "cos_wheel_pos_friction_force = "
                  << cos_wheel_pos_friction_force << '\n';
        float sin_wheel_pos_friction_force = PortedVector2CrossProduct(
            Vector2Normalize(wheel_position_rotated), friction_dir);
        std::cerr << "sin_wheel_pos_friction_force = "
                  << sin_wheel_pos_friction_force << '\n';

        float wheel_ang_acc_required_to_stop =
            delta_velocity_len / (wheel.radius * delta_time);
        std::cerr << "wheel_ang_acc_required_to_stop = "
                  << wheel_ang_acc_required_to_stop << '\n';
        float vehicle_acc_required_to_stop = delta_velocity_len / delta_time;
        std::cerr << "vehicle_acc_required_to_stop = "
                  << vehicle_acc_required_to_stop << '\n';
        /*float friction_required_to_stop =*/
        /*    (wheel_ang_acc_required_to_stop * wheel.moment_of_intertia /*/
        /*     (std::max<float>(std::abs(cos_wheel_heading_friction_force),*/
        /*                      1.0e-9) **/
        /*      wheel.radius)) +*/
        /*    (vehicle_acc_required_to_stop * mass /*/
        /*     std::max<float>(std::abs(cos_wheel_pos_friction_force), 1.0e-9));*/
        float friction_required_to_stop =
            (wheel_ang_acc_required_to_stop * wheel.moment_of_intertia /
             (std::max<float>(std::abs(cos_wheel_heading_friction_force),
                              1.0e-9) *
              wheel.radius)) +
            (vehicle_acc_required_to_stop * mass);
        /*float friction_required_to_stop =*/
        /*    (wheel_ang_acc_required_to_stop * wheel.moment_of_intertia /*/
        /*     (std::max<float>(cos_wheel_heading_friction_force, 1.0e-9) **/
        /*      wheel.radius)) +*/
        /*    (vehicle_acc_required_to_stop * mass);*/
        /*float friction_required_to_stop = vehicle_acc_required_to_stop *
         * mass;*/
        /*float friction_required_to_stop =*/
        /*    (wheel_ang_acc_required_to_stop * wheel.moment_of_intertia /*/
        /*     (wheel.radius)) +*/
        /*    (vehicle_acc_required_to_stop * mass);*/
        /*friction_required_to_stop = 0.05;*/
        std::cerr << "friction_required_to_stop = " << friction_required_to_stop
                  << '\n';

        float actual_friction;
        if (friction_required_to_stop <= max_static_friction) {
            actual_friction = friction_required_to_stop;
            std::cerr << "static_friction\n";
        } else {
            actual_friction = kinetic_friction;
            std::cerr << "kinetic_friction\n";
        }
        std::cerr << "actual_friction = " << actual_friction << '\n';

        // Apply forces and torques
        wheel.angular_velocity +=
            actual_friction * cos_wheel_heading_friction_force * wheel.radius *
            delta_time / (wheel.moment_of_intertia * 3.0);
        force = Vector2Add(force,
                           Vector2Scale(friction_dir, -actual_friction / 3.0));
        torque -= Vector2Length(wheel.position_relative) * actual_friction *
                  sin_wheel_pos_friction_force / 3.0;
    }

    applyForce(Vector3{force.x, 0.0, force.y}, delta_time);
    angular_momentum += torque * delta_time;

    std::cerr << "velocity_len = " << Vector3Length(computeVelocity()) << '\n';

    std::cerr << '\n';
}

void Vehicle::draw() {
    DrawModelEx(model, position, Vector3{0.0, -1.0, 0.0}, heading * 180.0 / PI,
                Vector3{1.0, 1.0, 1.0}, WHITE);
}
