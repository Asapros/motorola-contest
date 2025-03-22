#include <memory>
#include <vector>
#include "raylib.h"

#include "entity.hpp"
#include "rigidbody.hpp"
#include "vehicle.hpp"
#include "world.hpp"

int main() {
    const int screenWidth = 640;
    const int screenHeight = 480;
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetTargetFPS(60);
    InitWindow(screenWidth, screenHeight, "kubica");
    SetExitKey(KEY_ESCAPE);
    Camera camera = {{20.0f, 20.0f, 20.0f},
                     {0.0f, 0.0f, 0.0f},
                     {0.0f, 1.0f, 0.0f},
                     40.0f,
                     CAMERA_ORTHOGRAPHIC};
    Model model = LoadModel("assets/part.obj");

    Image image = GenImageCellular(20, 20, 10);
    Texture2D texture = LoadTextureFromImage(image);
    SetMaterialTexture(model.materials, MATERIAL_MAP_ALBEDO, texture);
    Vector3 position = {0.0f, 2.0f, 0.0f};

    World world = World();
    std::shared_ptr<Vehicle> entity = std::make_shared<Vehicle>(
        model, position, 1.0, 1.0,
        std::vector<Wheel>{{0.02, 0.0, 0.2, 0.0, {0.0, 0.5}},
                           {0.02, 0.0, 0.2, 0.0, {0.0, -0.5}}});
    world.spawnEntity(std::dynamic_pointer_cast<Entity>(entity));

    while (!WindowShouldClose()) {
        float delta_time = GetFrameTime();

        if (IsKeyDown(KEY_A)) {
            entity->applyForce({0.0, 0.005, 0.0}, delta_time);
        }
        if (IsKeyDown(KEY_W)) {
            for (auto& wheel : entity->wheels) {
                wheel.angular_velocity += 0.02 * delta_time;
            }
        }
        if (IsKeyDown(KEY_S)) {
            for (auto& wheel : entity->wheels) {
                wheel.angular_velocity -= 0.02 * delta_time;
            }
        }

        world.update(delta_time);

        BeginDrawing();
        ClearBackground(GetColor(0x181818ff));
        BeginMode3D(camera);

        world.draw();

        EndMode3D();
        EndDrawing();
    }

    UnloadModel(model);
    CloseWindow();
}
