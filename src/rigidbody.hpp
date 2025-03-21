#pragma once

#include "entity.hpp"

class Rigidbody : public Entity {
   private:
    Vector3 momentum;

   public:
    float mass;

    Rigidbody(Model& model, Vector3 position, float mass);

    void update();
    void applyForce(Vector3 force, float time);
    Vector3 computeVelocity();
};
