#include <string>

#include <raylib.h>

#include "game.hpp"
#include "vehicle.hpp"
#include "world.hpp"

void Game::update(float delta_time) {
    if (state == GameState::MainMenu) {
        if (IsKeyPressed(KEY_SPACE)) {
            state = GameState::InGame;
        }
    } else if (state == GameState::InGame) {
        if (!world.has_value()) {
            world = World();
        }

        if (IsKeyPressed(KEY_P)) {
            paused = !paused;
        }

        if (!paused) {
            world->update(delta_time);
        };
    }
}

void Game::draw() {
    if (state == GameState::MainMenu) {
        DrawText("Main menu", 20, 20, 24, WHITE);
        DrawText("Press space to play", 20, 60, 16, WHITE);
    } else if (state == GameState::InGame) {
        if (world.has_value()) {
            world->draw();
        }
    }
}

void Game::loadLevel(std::string level) {
    if (level == "test") {
        std::shared_ptr<Vehicle> entity = std::make_shared<Vehicle>(
            model, position, 10.0, 10.0,
            std::vector<Wheel>{{1.0, 0.0, 0.2, 0.0, {0.5, 1.0}},
                               {1.0, 0.0, 0.2, 0.0, {0.5, -1.0}},
                               {1.0, 0.0, 0.2, 0.0, {-0.5, 1.0}},
                               {1.0, 0.0, 0.2, 0.0, {-0.5, -1.0}}},
            std::move(vehicle_controller));
        world.spawnEntity(std::dynamic_pointer_cast<Entity>(entity));
    }
}
