#include <memory>

#include "entity.hpp"
#include "world.hpp"

World::World() {}

void World::update(float delta_time) {
    for (auto& [entityId, entity] : entities) {
        entity->update(delta_time);
    }
}

void World::draw() {
    DrawGrid(25, 2.0);
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
