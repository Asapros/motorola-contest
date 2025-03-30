#pragma once

#include <vector>

#include "raylib.h"

#include "entity.hpp"
#include "model_manager.hpp"

class Collidable : public Entity {
   private:
    bool checkAtAngle(Collidable& other, float angle);

   public:
    std::vector<Vector2> /*What a coincidence*/ collider_vertices;

    Collidable(std::shared_ptr<ModelWrapper> model,
               Vector3 position,
               float heading,
               std::vector<Vector2> collider_vertices);

    bool checkCollision(Collidable& other);
};
