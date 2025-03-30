#pragma once

#include <vector>

#include "raylib.h"

#include "collidable.hpp"

class Rigidbody : public Collidable {
   protected:
    Vector3 momentum;
    float mass;
    float angular_momentum;
    float moment_of_intertia;

   public:
    Rigidbody(std::shared_ptr<ModelWrapper> model,
              Vector3 position,
              float heading,
              std::vector<Vector2> collider_vertices,
              float mass,
              float moment_of_intertia);

    void update(float delta_time);
    void applyForce(Vector3 force, float time);
    void applyTorque(float torque, float time);
    Vector3 computeVelocity();
};
