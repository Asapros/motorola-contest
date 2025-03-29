#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

#include "raylib.h"

#include "controller.hpp"
#include "debug.hpp"
#include "entity.hpp"
#include "game.hpp"
#include "model_manager.hpp"
#include "rigidbody.hpp"
#include "vehicle.hpp"
#include "world.hpp"

const int screenWidth = 640;
const int screenHeight = 480;

int main() {
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
