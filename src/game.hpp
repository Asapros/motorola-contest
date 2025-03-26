#pragma once

#include <optional>
#include <string>

#include "world.hpp"
#include "model_manager.hpp"

enum class GameState {
    MainMenu,
    InGame,
};

class Game {
   private:
    ModelManager modelManager;
    void loadLevel(std::string level);
    Camera camera;
    bool showDebug;

   public:
    std::optional<World> world;
    GameState state;
    bool paused;

    Game();
    void update(float delta_time);
    void draw();
};
