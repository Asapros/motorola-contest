#include "rigidbody.hpp"
#include "raylib.h"
#include "raymath.h"

Rigidbody::Rigidbody(Model& model, Vector3 position, float mass)
    : Entity(model, position), momentum({0, 0, 0}), mass(mass) {};

void Rigidbody::update() {
    Entity::update();

    Vector3 velocity = computeVelocity();
    this->position = Vector3Add(this->position, velocity);
}

void Rigidbody::applyForce(Vector3 force, float time) {
    this->momentum = Vector3Add(this->momentum, Vector3Scale(force, time));
}

Vector3 Rigidbody::computeVelocity() {
    return Vector3Scale(this->momentum, 1.0 / this->mass);
}
