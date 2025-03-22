#pragma once

#include <vector>

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
    void update(float delta_time);
    void draw();
    Vehicle(Model& model,
            Vector3 position,
            float mass,
            float moment_of_intertia,
            std::vector<Wheel> wheels);
};
