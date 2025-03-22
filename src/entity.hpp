#pragma once

#include <cstdint>

#include "raylib.h"

#include "common.hpp"

class World;

class Entity {
   private:
   public:
    World* world;
    EntityId eid;
    Model& model;
    Vector3 position;

    Entity(Model& model, Vector3 position);

    virtual void update(float delta_time);
    void draw();
};
