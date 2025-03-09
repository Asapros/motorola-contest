#pragma once

#include <vector>
#include <map>

#include "common.hpp"

class Entity;

class World {
    private:
	EntityId next_eid;
    
    public:
    std::map<EntityId, Entity> entities;

    World();
    EntityId newEntityId();
    void spawnEntity(Entity entity);
	void update();
    void draw();
};