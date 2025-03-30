#include <iostream>
#include <memory>

#include "raylib.h"
#include "raymath.h"

#include "collidable.hpp"
#include "rigidbody.hpp"
#include "world.hpp"

Rigidbody::Rigidbody(std::shared_ptr<ModelWrapper> model,
                     Vector3 position,
                     float heading,
                     std::vector<Vector2> collider_vertices,
                     float mass,
                     float moment_of_intertia)
    : Collidable(model, position, heading, collider_vertices),
      momentum({0, 0, 0}),
      mass(mass),
      angular_momentum(0.0f),
      moment_of_intertia(moment_of_intertia) {};

void Rigidbody::update(float delta_time) {
    Entity::update(delta_time);

    for (const auto& entity : world->entities) {
        if (entity.second.get() == this) {
            // std::cout << "Self\n";
            continue;
        }
        std::cout << "Before dynamic_pointer_cast...\n";
        std::shared_ptr<Collidable> entity_collidable =
            std::dynamic_pointer_cast<Collidable>(entity.second);
        std::cout << "After dynamic_pointer_cast!\n";
        if (entity_collidable) {
            std::cout << "Found collidable\n";
            if (checkCollision(*entity_collidable)) {
                std::cout << "Collision! " << eid << ", " << entity.second->eid
                          << '\n';
            }
        }
    }

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
