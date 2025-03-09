#include "raylib.h"

int main()
{
    const int screenWidth = 1000;
    const int screenHeight = 1000;
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "kubica");
    SetExitKey(KEY_ESCAPE);
    Camera camera = {
        { 20.0f, 20.0f, 20.0f },
        { 0.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f },
        20.0f,
        CAMERA_ORTHOGRAPHIC
    };
    Model model = LoadModel("../../assets/part.obj");
    Texture2D texture = LoadTexture("../../assets/texture.jpg");
    model.materials[0].maps->texture = texture;
    Vector3 position = { 0.0f, 2.0f, 0.0f };
    
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(GetColor(0x181818ff));
        BeginMode3D(camera);

        DrawModel(model, {0,0,0}, 1.0f, WHITE);

        EndMode3D();
        EndDrawing();
    }

    UnloadModel(model);
    CloseWindow();
}
