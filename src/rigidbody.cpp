#include "rigidbody.hpp"
#include "raylib.h"
#include "raymath.h"

Rigidbody::Rigidbody(Model& model, Vector3 position, unsigned int mass)
    : Entity(model, position), velocity({0, 0, 0}), mass(mass) {};

void Rigidbody::update() {
    this->position = Vector3Add(this->position, this->velocity);
}

void Rigidbody::applyForce(Vector3 force) {
    this->velocity =
        Vector3Add(this->velocity, Vector3Scale(force, 1 / this->mass));
}
