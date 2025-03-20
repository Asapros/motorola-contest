#include "world.hpp"
#include "entity.hpp"

World::World() {}

void World::update() {
    for (auto& [entityId, entity] : entities) {
        entity.update();
    }
}

void World::draw() {
    for (auto& [entityId, entity] : entities) {
        entity.draw();
    }
}

EntityId World::newEntityId() {
    auto id = this->next_eid;
    this->next_eid++;
    return id;
}

void World::spawnEntity(Entity entity) {
    entity.eid = newEntityId();
    entity.world = this;
    this->entities.emplace(entity.eid, entity);
}
