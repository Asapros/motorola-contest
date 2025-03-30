#pragma once

#include <memory>
#include <optional>
#include <string>

#include "model_manager.hpp"
#include "ui.hpp"
#include "world.hpp"

class Game {
   private:
    ModelManager modelManager;
    void loadLevel(std::string level);
    Camera camera;
    bool showDebug;
    std::optional<EntityId> playerId;

   public:
    std::shared_ptr<World> world;
    UiManager ui;
    GameState state;
    bool paused;

    Game();
    void update(float delta_time);
    void draw();
};
