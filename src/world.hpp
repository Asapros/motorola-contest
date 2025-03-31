#pragma once

#include <array>
#include <map>
#include <memory>
#include <vector>

#include "raylib.h"

#include "common.hpp"

class Entity;

struct GroundMaterial {
    float static_friction_coef;
    float kinetic_friction_coef;
    Color color;
};

struct CheckpointZone {
    std::array<Vector2, 4> vertices;

    bool isPointIn(Vector2 point);
};

constexpr float grid_cell_size = 5.0f;

class World {
   private:
    EntityId next_eid;
    EntityId newEntityId();

   public:
    std::map<EntityId, std::shared_ptr<Entity>> entities;
    std::vector<Vector2> ai_line;
    std::vector<std::vector<char>> materials;
    std::map<int, CheckpointZone> checkpoints;

    World();
    EntityId spawnEntity(std::shared_ptr<Entity> entity);
    void update(float delta_time);
    void draw();
    GroundMaterial getMaterialAtCell(int ix, int iy);
    GroundMaterial getMaterialAtPosition(Vector2 pos);
};
