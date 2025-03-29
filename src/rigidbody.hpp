#pragma once

#include "entity.hpp"

class Rigidbody : public Entity {
   protected:
    Vector3 momentum;
    float mass;
    float angular_momentum;
    float moment_of_intertia;

   public:
    Rigidbody(std::shared_ptr<ModelWrapper> model,
              Vector3 position,
              float heading,
              float mass,
              float moment_of_intertia);

    void update(float delta_time);
    void applyForce(Vector3 force, float time);
    void applyTorque(float torque, float time);
    Vector3 computeVelocity();
};
