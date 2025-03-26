#include <algorithm>
#include <cmath>
#include <memory>
#include <vector>
#include <iostream>

#include "controller.hpp"
#include "raylib.h"

#include "entity.hpp"
#include "rigidbody.hpp"
#include "vehicle.hpp"
#include "world.hpp"
#include "model_manager.hpp"
#include "game.hpp"

int main() {
    const int screenWidth = 640;
    const int screenHeight = 480;
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetTargetFPS(60);
    InitWindow(screenWidth, screenHeight, "kubica");
    SetExitKey(KEY_ESCAPE);
    Game game = Game();

    while (!WindowShouldClose()) {
        float delta_time = GetFrameTime();
        game.update(delta_time);

        BeginDrawing();
        ClearBackground(GetColor(0x181818ff));

        game.draw();

        EndDrawing();
    }

    CloseWindow();
}
