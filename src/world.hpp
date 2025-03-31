#pragma once

#include <map>
#include <memory>
#include <vector>

#include "common.hpp"

class Entity;

class World {
   private:
    EntityId next_eid;
    EntityId newEntityId();

   public:
    std::map<EntityId, std::shared_ptr<Entity>> entities;
    std::vector<std::vector<char>> materials;

    World();
    EntityId spawnEntity(std::shared_ptr<Entity> entity);
    void update(float delta_time);
    void draw();
};
