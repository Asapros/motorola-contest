#pragma once

#include <map>
#include <memory>
#include <vector>

#include "common.hpp"

class Entity;

class World {
   private:
    EntityId next_eid;

   public:
    std::map<EntityId, std::shared_ptr<Entity>> entities;

    World();
    EntityId newEntityId();
    void spawnEntity(std::shared_ptr<Entity> entity);
    void update(float delta_time);
    void draw();
};
