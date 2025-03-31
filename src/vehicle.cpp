#include <cmath>
#include <iostream>
#include <memory>

#include <raylib.h>
#include <raymath.h>

#include "common.hpp"
#include "debug.hpp"
#include "game.hpp"
#include "rigidbody.hpp"
#include "vehicle.hpp"
#include "world.hpp"

// Backported from https://github.com/raysan5/raylib/pull/4520 - there was no
// stable release since that pull request
float PortedVector2CrossProduct(Vector2 v1, Vector2 v2) {
    float result = (v1.x * v2.y - v1.y * v2.x);

    return result;
}

Vehicle::Vehicle(std::shared_ptr<ModelWrapper> model,
                 Vector3 position,
                 float heading,
                 std::vector<Vector2> collider_vertices,
                 float mass,
                 float moment_of_intertia,
                 std::vector<Wheel> wheels,
                 std::unique_ptr<Controller> controller)
    : Rigidbody(model,
                position,
                heading,
                collider_vertices,
                mass,
                moment_of_intertia),
      wheels(wheels),
      controller(std::move(controller)),
      gear(0) {}

void Vehicle::update(float delta_time) {
    Rigidbody::update(delta_time);

    Controls controls = controller->computeControls(world, eid);
    gear = controls.gear;

    debugValues["ctl_gear"] = std::to_string(gear);

    // TODO: don't hardcode it here; make it possible to drive only subset of
    // wheels (i.e. front ones only)
    const float engine_torque = 2.0;
    const float steering_rate = 1.2;
    const float max_steering = 0.8;
    for (uint32_t i = 0; i < wheels.size(); i++) {
        wheels[i].angular_velocity += controls.accelerator * engine_torque *
                                      delta_time / wheels[i].moment_of_intertia;
        if (i < 2) {
            float target_steering = max_steering * controls.steering;
            if (target_steering > wheels[i].steering) {
                wheels[i].steering += steering_rate * delta_time;
            } else {
                wheels[i].steering -= steering_rate * delta_time;
            }
        }
    }

    // Angular momentum drag
    angular_momentum *= std::pow(0.85, delta_time);
    debugValues["phys_ang_mom"] = std::to_string(angular_momentum);

    debugValues["phys_direction"] = std::to_string(heading);

    Vector3 velocity = computeVelocity();

    float grav_force = mass * GRAVITY_ACCELERATION;

    float torque = 0.0;
    Vector2 force = {0.0, 0.0};

    for (auto& wheel : wheels) {
        // Drag
        wheel.angular_velocity *= std::pow(0.95, delta_time);

        // std::cerr << "angular_velocity = " << wheel.angular_velocity << '\n';
        // std::cerr << "steering = " << wheel.steering << '\n';

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

        // std::cerr << "delta_velocity_len = " << delta_velocity_len << '\n';

        // TODO: determine from tire and ground material
        GroundMaterial ground_mat =
            world->getMaterialAtPosition({position.x, position.z});
        // float static_friction_coef = 0.8;
        // float kinetic_friction_coef = 0.4;

        float max_static_friction =
            ground_mat.static_friction_coef * grav_force / wheels.size();
        float kinetic_friction =
            ground_mat.kinetic_friction_coef * grav_force / wheels.size();

        Vector2 friction_dir = Vector2Normalize(delta_velocity);

        Vector2 wheel_position_rotated =
            Vector2Rotate(wheel.position_relative, heading);

        float cos_wheel_heading_friction_force =
            Vector2DotProduct(wheel_heading_vec, friction_dir);
        // std::cerr << "cos_wheel_heading_friction_force = "
        //           << cos_wheel_heading_friction_force << '\n';
        // float cos_wheel_pos_friction_force = Vector2DotProduct(
        //    Vector2Normalize(wheel_position_rotated), friction_dir);
        // std::cerr << "cos_wheel_pos_friction_force = "
        //           << cos_wheel_pos_friction_force << '\n';
        float sin_wheel_pos_friction_force = PortedVector2CrossProduct(
            Vector2Normalize(wheel_position_rotated), friction_dir);
        // std::cerr << "sin_wheel_pos_friction_force = "
        //           << sin_wheel_pos_friction_force << '\n';

        float wheel_ang_acc_required_to_stop =
            delta_velocity_len / (wheel.radius * delta_time);
        // std::cerr << "wheel_ang_acc_required_to_stop = "
        //           << wheel_ang_acc_required_to_stop << '\n';
        float vehicle_acc_required_to_stop = delta_velocity_len / delta_time;
        // std::cerr << "vehicle_acc_required_to_stop = "
        //           << vehicle_acc_required_to_stop << '\n';
        float friction_required_to_stop =
            (wheel_ang_acc_required_to_stop * wheel.moment_of_intertia /
             (std::max<float>(std::abs(cos_wheel_heading_friction_force),
                              1.0e-9) *
              wheel.radius)) +
            (vehicle_acc_required_to_stop * mass);
        // std::cerr << "friction_required_to_stop = " <<
        // friction_required_to_stop
        //           << '\n';

        float actual_friction;
        if (friction_required_to_stop <= max_static_friction) {
            actual_friction = friction_required_to_stop;
            // std::cerr << "static_friction\n";
        } else {
            actual_friction = kinetic_friction;
            // std::cerr << "kinetic_friction\n";
        }
        // std::cerr << "actual_friction = " << actual_friction << '\n';

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
    applyTorque(torque, delta_time);

    debugValues["phys_velocity"] =
        std::to_string(Vector3Length(computeVelocity()));
}
