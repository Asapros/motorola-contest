#include "raylib.h"

#include "entity.hpp"
#include "world.hpp"

Entity::Entity(std::shared_ptr<ModelWrapper> model, Vector3 position)
    : world(nullptr), eid(0), model(model), position(position) {}

Entity::~Entity() {}

void Entity::update([[maybe_unused]] float delta_time) {}

void Entity::draw() {
    DrawModel(model->model, position, 1.0f, WHITE);
};
