#pragma once

#include <cstdint>

#include "raylib.h"

#include "common.hpp"
#include "model_manager.hpp"

class World;

class Entity {
   private:
   public:
    World* world;
    EntityId eid;
    std::shared_ptr<ModelWrapper> model;
    Vector3 position;
    float heading;

    Entity(std::shared_ptr<ModelWrapper> model,
           Vector3 position,
           float heading = 0.0f);
    virtual ~Entity();

    virtual void update(float delta_time);
    virtual void draw();
};
