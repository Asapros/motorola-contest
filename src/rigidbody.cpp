#include "rigidbody.hpp"
#include "raylib.h"
#include "raymath.h"

Rigidbody::Rigidbody(std::shared_ptr<ModelWrapper> model,
                     Vector3 position,
                     float heading,
                     float mass,
                     float moment_of_intertia)
    : Entity(model, position, heading),
      momentum({0, 0, 0}),
      mass(mass),
      angular_momentum(0.0f),
      moment_of_intertia(moment_of_intertia) {};

void Rigidbody::update(float delta_time) {
    Entity::update(delta_time);

    Vector3 velocity = computeVelocity();
    position = Vector3Add(position, velocity);

    heading += angular_momentum / moment_of_intertia;
    heading = fmod(heading, 2 * PI);
}

void Rigidbody::applyForce(Vector3 force, float time) {
    momentum = Vector3Add(momentum, Vector3Scale(force, time));
}

void Rigidbody::applyTorque(float torque, float time) {
    angular_momentum += torque * time;
}

Vector3 Rigidbody::computeVelocity() {
    return Vector3Scale(momentum, 1.0 / mass);
}
