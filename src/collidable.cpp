#include <algorithm>
#include <vector>

#include "raylib.h"
#include "raymath.h"

#include "collidable.hpp"
#include "entity.hpp"

Collidable::Collidable(std::shared_ptr<ModelWrapper> model,
                       Vector3 position,
                       float heading,
                       std::vector<Vector2> collider_vertices)
    : Entity(model, position, heading), collider_vertices(collider_vertices) {}

void castAtAngle(Collidable& collidable,
                 float angle,
                 std::vector<float>& result) {
    for (const Vector2& vert : collidable.collider_vertices) {
        Vector2 vert_transformed =
            Vector2Add(Vector2Rotate(vert, -angle),
                       Vector2{collidable.position.x, collidable.position.y});
        result.push_back(vert_transformed.x);
    }
}

bool Collidable::checkAtAngle(Collidable& other, float angle) {
    std::vector<float> this_verts_casted;
    this_verts_casted.reserve(collider_vertices.size());
    castAtAngle(*this, angle, this_verts_casted);

    float this_verts_casted_min =
        *std::min_element(this_verts_casted.begin(), this_verts_casted.end());
    float this_verts_casted_max =
        *std::max_element(this_verts_casted.begin(), this_verts_casted.end());

    std::vector<float> other_verts_casted;
    other_verts_casted.reserve(other.collider_vertices.size());
    castAtAngle(other, angle, other_verts_casted);

    float other_verts_casted_min =
        *std::min_element(other_verts_casted.begin(), other_verts_casted.end());
    float other_verts_casted_max =
        *std::max_element(other_verts_casted.begin(), other_verts_casted.end());

    return this_verts_casted_max < other_verts_casted_min ||
           this_verts_casted_min > other_verts_casted_max;
}

bool Collidable::checkCollision(Collidable& other) {
    for (uint32_t i = 0; i < collider_vertices.size(); i++) {
        float angle = Vector2LineAngle(
            collider_vertices[i],
            collider_vertices[(i + 1) % collider_vertices.size()]);
        if (!checkAtAngle(other, angle + PI / 2)) {
            return false;
        }
    }

    for (uint32_t i = 0; i < other.collider_vertices.size(); i++) {
        float angle = Vector2LineAngle(
            other.collider_vertices[i],
            other.collider_vertices[(i + 1) % other.collider_vertices.size()]);
        if (!checkAtAngle(other, angle + PI / 2)) {
            return false;
        }
    }

    return true;
}
