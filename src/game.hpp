#pragma once

#include <memory>
#include <optional>
#include <string>

#include "model_manager.hpp"
#include "ui.hpp"
#include "world.hpp"

class Game {
   private:
    std::shared_ptr<ModelManager> model_manager;
    void loadLevel(std::string level);
    Camera camera;
    bool show_debug;
    std::optional<EntityId> player_id;

   public:
    std::shared_ptr<World> world;
    UiManager ui;
    GameState state;
    bool paused;

    Game();
    void update(float delta_time);
    void draw();
};
