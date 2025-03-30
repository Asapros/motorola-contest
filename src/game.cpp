#include <format>
#include <string>
#include <vector>

#include <raylib.h>

#include "debug.hpp"
#include "game.hpp"
#include "ui.hpp"
#include "vehicle.hpp"
#include "world.hpp"

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
    } else if (state == GameState::InGame) {
        if (!world) {
            loadLevel("test");
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
        ui.drawMenu();
        return;
    }
    if (state == GameState::InGame) {
        if (!world)
            return;
        if (!playerId.has_value())
            return;

        BeginMode3D(camera);
        world->draw();
        EndMode3D();

        ui.drawUi(*world, playerId.value());

        // TODO move to dedicated function
        // if (IsKeyPressed(KEY_F3)) {
        //     this->showDebug = !this->showDebug;
        // }

        // if (!this->showDebug) return;
        // const int debugValuesSize = 15;
        // int offset = 0;
        // for (const auto &pair : debugValues) {
        //     DrawText(std::format("{}: {}", pair.first, pair.second).c_str(),
        //     0, offset * debugValuesSize, debugValuesSize, VIOLET); offset++;
        // }
    }
}

void Game::loadLevel(std::string level) {
    debugLog("MENU", std::format("loading level '{}'", level));
    world = std::make_shared<World>();
    std::unique_ptr<Controller> vehicle_controller =
        std::make_unique<PlayerController>();
    Vector3 position = {0.0, 0.0, 0.0};
    std::shared_ptr<Vehicle> entity = std::make_shared<Vehicle>(
        modelManager.getModel("assets/car_prototype.glb"), position, 0.0,
        std::vector<Vector2>(), 10.0, 10.0,
        std::vector<Wheel>{{1.0, 0.0, 0.2, 0.0, {0.5, 1.0}},
                           {1.0, 0.0, 0.2, 0.0, {0.5, -1.0}},
                           {1.0, 0.0, 0.2, 0.0, {-0.5, 1.0}},
                           {1.0, 0.0, 0.2, 0.0, {-0.5, -1.0}}},
        std::move(vehicle_controller));
    playerId = world->spawnEntity(std::dynamic_pointer_cast<Entity>(entity));
}
