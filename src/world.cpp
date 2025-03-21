#include <memory>

#include "entity.hpp"
#include "world.hpp"

World::World() {}

void World::update() {
    for (auto& [entityId, entity] : entities) {
        entity->update();
    }
}

void World::draw() {
    for (auto& [entityId, entity] : entities) {
        entity->draw();
    }
}

EntityId World::newEntityId() {
    auto id = this->next_eid;
    this->next_eid++;
    return id;
}

void World::spawnEntity(std::shared_ptr<Entity> entity) {
    entity->eid = newEntityId();
    entity->world = this;
    this->entities.emplace(entity->eid, entity);
}
