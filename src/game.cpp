#include <string>

#include <raylib.h>

#include "game.hpp"
#include "vehicle.hpp"
#include "world.hpp"
#include "debug.hpp"
#include <format>

Game::Game() : modelManager(ModelManager()), state(GameState::MainMenu) {
    this->camera = {{30.0f, 30.0f, 30.0f},
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
    } else if (state == GameState::InGame) {
        if (!world.has_value()) {
            this->loadLevel("test");
        }

        if (IsKeyPressed(KEY_SPACE)) {
            state = GameState::MainMenu;
        }

        if (state == GameState::InGame) {
            world->update(delta_time);
        };
    }
}

void Game::draw() {
    if (state == GameState::MainMenu) {
        DrawText("Main menu", 0, 0, 20, RED);
        DrawText("Press space to play", 20, 60, 16, WHITE);
        return;
    }
    if (state == GameState::InGame) {
        if (!world.has_value()) return;

        BeginMode3D(camera);
        DrawGrid(25, 2.0);
        world->draw();
        EndMode3D();
    }
}

void Game::loadLevel(std::string level) {
    debugLog("MENU", std::format("loading level '{}'", level));
    World world = World();
    std::unique_ptr<Controller> vehicle_controller =
        std::make_unique<PlayerController>();
    Vector3 position = {0.0, 0.0, 0.0};
    std::shared_ptr<Vehicle> entity = std::make_shared<Vehicle>(
        this->modelManager.getModel("assets/car_prototype.glb"), position, 10.0, 10.0,
        std::vector<Wheel>{{1.0, 0.0, 0.2, 0.0, {0.5, 1.0}},
                           {1.0, 0.0, 0.2, 0.0, {0.5, -1.0}},
                           {1.0, 0.0, 0.2, 0.0, {-0.5, 1.0}},
                           {1.0, 0.0, 0.2, 0.0, {-0.5, -1.0}}},
        std::move(vehicle_controller));
    world.spawnEntity(std::dynamic_pointer_cast<Entity>(entity));
    this->world = world;
}
