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
   private:
    float heading;
    float angular_momentum;
    float moment_of_intertia;

   public:
    std::vector<Wheel> wheels;
    std::unique_ptr<Controller> controller;
    void update(float delta_time);
    void draw();
    Vehicle(std::shared_ptr<ModelWrapper> model,
            Vector3 position,
            float mass,
            float moment_of_intertia,
            std::vector<Wheel> wheels,
            std::unique_ptr<Controller> controller);
};
