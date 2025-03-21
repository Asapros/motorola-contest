#pragma once

#include <vector>

#include "entity.hpp"
#include "rigidbody.hpp"

struct Wheel {
    float moment_of_intertia;
    float angular_velocity;
    float radius;
};

class Vehicle : Rigidbody {
   private:
    float heading;
    float angular_momentum;
    float moment_of_intertia;
    std::vector<Wheel> wheels;

   public:
    void update();
};
