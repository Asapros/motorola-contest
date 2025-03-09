#include "raylib.h"

#include "world.hpp"
#include "entity.hpp"

Entity::Entity(Model& _model, Vector3 position): model(_model) {
    // this->model = model;
    this->world = nullptr;
    this->eid = 0;
    this->position = position;
};

void Entity::update() {

}

void Entity::draw() {
    DrawModel(model, position, 1.0f, WHITE);
};