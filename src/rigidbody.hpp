#pragma once

#include "entity.hpp"

class Rigidbody : public Entity {
    Vector3 velocity;

    public:
    unsigned int mass;

    Rigidbody(Model& model, Vector3 position, unsigned int mass);

    void update();
    void applyForce(Vector3 force);
};