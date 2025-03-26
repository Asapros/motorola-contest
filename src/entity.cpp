#include "raylib.h"

#include "entity.hpp"
#include "world.hpp"

Entity::Entity(std::shared_ptr<ModelWrapper> model, Vector3 position)
: model(model), world(nullptr), eid(0), position(position) {};

void Entity::update(float delta_time) {}

void Entity::draw() {
    DrawModel(model->model, position, 1.0f, WHITE);
};
