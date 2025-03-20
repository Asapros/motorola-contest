#include "entity.hpp"
#include "raylib.h"
#include "rigidbody.hpp"
#include "world.hpp"

int main() {
    const int screenWidth = 1000;
    const int screenHeight = 1000;
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "kubica");
    SetExitKey(KEY_ESCAPE);
    Camera camera = {{20.0f, 20.0f, 20.0f},
                     {0.0f, 0.0f, 0.0f},
                     {0.0f, 1.0f, 0.0f},
                     20.0f,
                     CAMERA_ORTHOGRAPHIC};
    Model model = LoadModel("assets/part.obj");

    Image image = GenImageCellular(20, 20, 10);
    Texture2D texture = LoadTextureFromImage(image);
    SetMaterialTexture(model.materials, MATERIAL_MAP_ALBEDO, texture);
    Vector3 position = {0.0f, 2.0f, 0.0f};

    World world = World();
    Rigidbody entity = Rigidbody(model, position, 1);
    world.spawnEntity(entity);

    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_A)) {
            entity.applyForce({0.0, 1.0, 0.0});
        }
        world.update();

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
