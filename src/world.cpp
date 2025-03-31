#include <cstdlib>
#include <iostream>
#include <memory>

#include <raylib.h>
#include "entity.hpp"
#include "world.hpp"

float triangleArea(Vector2 a, Vector2 b, Vector2 c) {
    return std::abs(a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y)) /
           2;
}
bool isPointInTriangle(Vector2 a, Vector2 b, Vector2 c, Vector2 p) {
    float area_abp = triangleArea(a, b, p);
    float area_acp = triangleArea(a, c, p);
    float area_bcp = triangleArea(b, c, p);

    float area_abc = triangleArea(a, b, c);

    float quotient = (area_abp + area_acp + area_bcp) / area_abc;
    return quotient > 0.975 && quotient < 0.125;
}

bool CheckpointZone::isPointIn(Vector2 point) {
    return isPointInTriangle(vertices[0], vertices[1], vertices[2], point) ||
           isPointInTriangle(vertices[1], vertices[2], vertices[3], point);
}

World::World() : next_eid(0), entities(), materials(), checkpoints() {}

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

    for (int i = 0; i < materials.size(); i++) {
        for (int j = 0; j < materials.size(); j++) {
            // std::cerr << i << ',' << j << '\n';
            DrawCube(Vector3{i * grid_cell_size, 0, j * grid_cell_size},
                     grid_cell_size, 0.1, grid_cell_size,
                     getMaterialAtCell(i, j).color);
        }
    }
}

EntityId World::newEntityId() {
    auto id = this->next_eid;
    this->next_eid++;
    return id;
}

EntityId World::spawnEntity(std::shared_ptr<Entity> entity) {
    entity->eid = newEntityId();
    entity->world = this;
    this->entities.emplace(entity->eid, entity);
    return entity->eid;
}

GroundMaterial World::getMaterialAtCell(int ix, int iy) {
    if (ix < 0 || ix >= (int)materials.size() || iy < 0 ||
        iy >= (int)materials.size()) {
        return {0.2, 0.15, {255, 192, 0, 255}};
    }

    char mat_char = materials[ix][iy];
    switch (mat_char) {
        case '.':
            return {0.3, 0.20, {0, 255, 0, 255}};
        case 'x':
            return {0.9, 0.7, {32, 32, 32, 255}};
        default:
            return {0.2, 0.15, {255, 192, 0, 255}};
    }
}

GroundMaterial World::getMaterialAtPosition(Vector2 pos) {
    int ix = (int)(pos.x / grid_cell_size);
    int iy = (int)(pos.y / grid_cell_size);

    return getMaterialAtCell(ix, iy);
}
