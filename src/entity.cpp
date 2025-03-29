#include "raylib.h"

#include "entity.hpp"
#include "world.hpp"

Entity::Entity(std::shared_ptr<ModelWrapper> model,
               Vector3 position,
               float heading)
    : world(nullptr),
      eid(0),
      model(model),
      position(position),
      heading(heading) {}

Entity::~Entity() {}

void Entity::update([[maybe_unused]] float delta_time) {}

void Entity::draw() {
    DrawModelEx(model->model, position, Vector3{0.0, -1.0, 0.0},
                heading * 180.0 / PI, Vector3{1.0, 1.0, 1.0}, WHITE);
};
