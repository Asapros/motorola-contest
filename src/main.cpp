#include <cmath>
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
    Camera camera = {{30.0f, 30.0f, 30.0f},
                     {0.0f, 0.0f, 0.0f},
                     {0.0f, 1.0f, 0.0f},
                     60.0f,
                     CAMERA_ORTHOGRAPHIC};
    Model model = LoadModel("assets/car_prototype.glb");

    Image image = GenImageCellular(20, 20, 10);
    Texture2D texture = LoadTextureFromImage(image);
    SetMaterialTexture(model.materials, MATERIAL_MAP_ALBEDO, texture);
    Vector3 position = {0.0f, 0.0f, 0.0f};

    World world = World();
    std::shared_ptr<Vehicle> entity = std::make_shared<Vehicle>(
        model, position, 10.0, 10.0,
        std::vector<Wheel>{{1.0, 0.0, 0.2, 0.0, {0.5, 1.0}},
                           {1.0, 0.0, 0.2, 0.0, {0.5, -1.0}},
                           {1.0, 0.0, 0.2, 0.0, {-0.5, 1.0}},
                           {1.0, 0.0, 0.2, 0.0, {-0.5, -1.0}}});
    world.spawnEntity(std::dynamic_pointer_cast<Entity>(entity));

    while (!WindowShouldClose()) {
        float delta_time = GetFrameTime();

        /*if (IsKeyDown(KEY_A)) {*/
        /*    entity->applyForce({0.0, 0.005, 0.0}, delta_time);*/
        /*}*/
        for (uint32_t i = 0; i < entity->wheels.size(); i++) {
            auto& wheel = entity->wheels[i];
            const float engine_torque = 2.0;
            if (IsKeyDown(KEY_W)) {
                wheel.angular_velocity += engine_torque * delta_time;
            }
            if (IsKeyDown(KEY_S)) {
                wheel.angular_velocity -= engine_torque * delta_time;
            }
            if (i < 2) {
                const float steering_rate = 0.8;
                float target_steering;
                if (IsKeyDown(KEY_A)) {
                    target_steering = -0.8;
                } else if (IsKeyDown(KEY_D)) {
                    target_steering = 0.8;
                } else {
                    target_steering = 0.0;
                }

                if (target_steering > wheel.steering) {
                    wheel.steering += steering_rate * delta_time;
                } else {
                    wheel.steering -= steering_rate * delta_time;
                }
            }
        }

        world.update(delta_time);

        BeginDrawing();
        ClearBackground(GetColor(0x181818ff));
        BeginMode3D(camera);

        world.draw();

        DrawGrid(25, 2.0);

        EndMode3D();
        EndDrawing();
    }

    UnloadModel(model);
    CloseWindow();
}
