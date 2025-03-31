#include <format>
#include <memory>
#include <string>
#include <vector>

#include <raylib.h>

#include "ai.hpp"
#include "collidable.hpp"
#include "debug.hpp"
#include "game.hpp"
#include "model_manager.hpp"
#include "ui.hpp"
#include "vehicle.hpp"
#include "world.hpp"

GameState previousState;

Game::Game()
    : player_id(), world(), ui(UiManager()), state(GameState::MainMenu) {
    model_manager = std::make_shared<ModelManager>();
    camera = {{30.0f, 30.0f, 30.0f},
              {0.0f, 0.0f, 0.0f},
              {0.0f, 1.0f, 0.0f},
              60.0f,
              CAMERA_ORTHOGRAPHIC};
}

Game::~Game() {
    // Destroy World under the shared_ptr
    world.reset();
}

void Game::update(float delta_time) {
    if (state == GameState::MainMenu) {
        if (IsKeyPressed(KEY_SPACE)) {
            state = GameState::InGame;
        }
    } else if (state == GameState::InGame) {
        if (!world) {
            this->loadLevel("map_test");
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
    if (state != GameState::InGame && state != GameState::InSettings)
        previousState = state;

    if (state == GameState::MainMenu) {
        ui.drawMenu();
        state = ui.state;
        return;
    }
    if (state == GameState::InGame) {
        if (!world)
            return;
        if (!player_id.has_value())
            return;

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
        ui.drawUi(world.get(), player_id.value());
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
        model_manager->getModel("car_prototype.glb"), Vector3{0.0, 0.0, 0.0},
        0.0,
        std::vector<Vector2>{
            {-1.0, -1.0}, {-1.0, 1.0}, {1.0, 1.0}, {1.0, -1.0}},
        10.0, 10.0,
        std::vector<Wheel>{{1.0, 0.0, 0.2, 0.0, {0.5, 1.0}},
                           {1.0, 0.0, 0.2, 0.0, {0.5, -1.0}},
                           {1.0, 0.0, 0.2, 0.0, {-0.5, 1.0}},
                           {1.0, 0.0, 0.2, 0.0, {-0.5, -1.0}}},
        std::move(vehicle_controller));
    player_id = world->spawnEntity(std::dynamic_pointer_cast<Entity>(entity));

    // Some example objects
    std::unique_ptr<Controller> ai_controller = std::make_unique<StraightGuy>();

    std::shared_ptr<Vehicle> ai_player = std::make_shared<Vehicle>(
        model_manager->getModel("car_prototype.glb"), Vector3{0.0, 0.0, 0.0},
        0.0,
        std::vector<Vector2>{
            {-1.0, -1.0}, {-1.0, 1.0}, {1.0, 1.0}, {1.0, -1.0}},
        10.0, 10.0,
        std::vector<Wheel>{{1.0, 0.0, 0.2, 0.0, {0.5, 1.0}},
                           {1.0, 0.0, 0.2, 0.0, {0.5, -1.0}},
                           {1.0, 0.0, 0.2, 0.0, {-0.5, 1.0}},
                           {1.0, 0.0, 0.2, 0.0, {-0.5, -1.0}}},
        std::move(ai_controller));
    world->spawnEntity(ai_player);

    if (level != "test") {
        model_manager->loadMap(level, this);
    }
}
