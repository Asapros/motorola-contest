#pragma once

#include <optional>
#include <string>

#include "world.hpp"
#include "model_manager.hpp"
#include "ui.hpp"

class Game {
   private:
    ModelManager modelManager;
    void loadLevel(std::string level);
    Camera camera;
    bool showDebug;
    std::optional<EntityId> playerId;

   public:
    std::optional<World> world;
    UiManager ui;
    GameState state;
    bool paused;

    Game();
    void update(float delta_time);
    void draw();
};
