#pragma once

#include "entity.hpp"

class Rigidbody : public Entity {
    Vector3 velocity;

   public:
    float mass;

    Rigidbody(Model& model, Vector3 position, float mass);

    void update();
    void applyForce(Vector3 force);
};
