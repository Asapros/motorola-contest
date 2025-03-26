#pragma once

#include <optional>
#include <string>

#include "world.hpp"

enum class GameState {
    MainMenu,
    InGame,
};

class Game {
   private:
    void loadLevel(std::string level);

   public:
    std::optional<World> world;
    GameState state;
    bool paused;

    Game();
    void update(float delta_time);
    void draw();
};
