#include <format>
#include <string>
#include <vector>

#include <raylib.h>

#include "collidable.hpp"
#include "debug.hpp"
#include "game.hpp"
#include "ui.hpp"
#include "vehicle.hpp"
#include "world.hpp"

GameState previousState;

Game::Game()
    : modelManager(ModelManager()),
      showDebug(false),
      playerId(),
      world(),
      ui(UiManager()),
      state(GameState::MainMenu) {
    camera = {{30.0f, 30.0f, 30.0f},
              {0.0f, 0.0f, 0.0f},
              {0.0f, 1.0f, 0.0f},
              60.0f,
              CAMERA_ORTHOGRAPHIC};
}

void Game::update(float delta_time) {
    if (state == GameState::MainMenu) {
        if (IsKeyPressed(KEY_SPACE)) {
            state = GameState::InGame;
        }
    }
    else if (state == GameState::InGame) {
        if (!world) {
            this->loadLevel("test");
        }
        if (IsKeyPressed(KEY_ESCAPE)) {
            state = GameState::InPause;
        }

        if (state == GameState::InGame) {
            world->update(delta_time);
        };
    } else if (state == GameState::InPause) {
        if (IsKeyPressed(KEY_ESCAPE)) {
            state = GameState::InGame;
        }
    }
}

void Game::draw() {
    if(state != GameState::InGame && state != GameState::InSettings)
        previousState = state;

    if (state == GameState::MainMenu) {
        ui.drawMenu();
        state = ui.state;
        return;
    }
    if (state == GameState::InGame) {
        if (!world) return;
        if (!playerId.has_value()) return;

        // THIS IS A VERY BIG TODO; DO NOT TOUCH THIS
        // if (state == GameState::InPause) {
        //      BeginShaderMode(blur_shader);
                    BeginMode3D(camera);
                    world->draw();
                    EndMode3D();
        //      EndShaderMode();
        // }
        // else {
            // BeginMode3D(camera);
            // world->draw();
            // EndMode3D();
        // }
        ui.drawUi(*world, playerId.value());

        ui.drawMeter(69.0f, 1, 3.0f, {GetScreenWidth() - 100, GetScreenHeight() - 100}, 80, NULL, { 0, 20, 40, 60, 80, 100, 120 });
        ui.drawMeter(2137.0f, 1000, 0.0f, {GetScreenWidth() - 280, GetScreenHeight() - 100}, 80, "x1000", { 0, 1, 2, 3, 4, 5, 6 });

        // if (IsKeyPressed(KEY_F3)) { this->showDebug = !this->showDebug; } // TODO move to dedicated function

        // if (!this->showDebug) return;
        // const int debugValuesSize = 15;
        // int offset = 0;
        // for (const auto &pair : debugValues) {
        //     DrawText(std::format("{}: {}", pair.first, pair.second).c_str(),
        //     0, offset * debugValuesSize, debugValuesSize, VIOLET); offset++;
        // }
    }
    if (state == GameState::InPause) {
        ui.drawPauseMenu();
        state = ui.state;
        return;
    }
    if (state == GameState::InSettings) {
        ui.drawSettings(previousState);
        state = ui.state;
        return;
    }
}


void Game::loadLevel(std::string level) {
    debugLog("MENU", std::format("loading level '{}'", level));
    world = std::make_shared<World>();

    std::unique_ptr<Controller> vehicle_controller =
        std::make_unique<PlayerController>();
    std::shared_ptr<Vehicle> entity = std::make_shared<Vehicle>(
        modelManager.getModel("assets/car_prototype.glb"),
        Vector3{0.0, 0.0, 0.0}, 0.0,
        std::vector<Vector2>{
            {-1.0, -1.0}, {-1.0, 1.0}, {1.0, 1.0}, {1.0, -1.0}},
        10.0, 10.0,
        std::vector<Wheel>{{1.0, 0.0, 0.2, 0.0, {0.5, 1.0}},
                           {1.0, 0.0, 0.2, 0.0, {0.5, -1.0}},
                           {1.0, 0.0, 0.2, 0.0, {-0.5, 1.0}},
                           {1.0, 0.0, 0.2, 0.0, {-0.5, -1.0}}},
        std::move(vehicle_controller));
    playerId = world->spawnEntity(std::dynamic_pointer_cast<Entity>(entity));

    // Some example objects
    std::shared_ptr<Collidable> obstacle_1 = std::make_shared<Collidable>(
        modelManager.getModel("assets/car_prototype.glb"),
        Vector3{6.0, 0.0, 3.0}, 0.1,
        std::vector<Vector2>{
            {-1.0, -1.0}, {-1.0, 1.0}, {1.0, 1.0}, {1.0, -1.0}});
    world->spawnEntity(obstacle_1);
}
