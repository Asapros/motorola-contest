#pragma once

#include <memory>
#include <vector>

#include "controller.hpp"
#include "entity.hpp"
#include "rigidbody.hpp"

struct Wheel {
    float moment_of_intertia;
    float angular_velocity;
    float radius;
    float steering;
    Vector2 position_relative;
};

class Vehicle : public Rigidbody {
   public:
    int gear;
    std::vector<Wheel> wheels;
    std::unique_ptr<Controller> controller;
    void update(float delta_time);
    Vehicle(std::shared_ptr<ModelWrapper> model,
            Vector3 position,
            float heading,
            std::vector<Vector2> collider_vertices,
            float mass,
            float moment_of_intertia,
            std::vector<Wheel> wheels,
            std::unique_ptr<Controller> controller);
};
