#include "raylib.h"

#include "world.hpp"
#include "entity.hpp"

Entity::Entity(Model& model, Vector3 position): model(model), world(nullptr), eid(0), position(position) {};

void Entity::update() {

}

void Entity::draw() {
    DrawModel(model, position, 1.0f, WHITE);
};