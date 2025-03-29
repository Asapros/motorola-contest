#pragma once

#include "common.hpp"
#include "world.hpp"
#include "raygui.h"

struct Styles {
    int BORDER_COLOR = 0x00FF00FF;
    int BASE_COLOR = 0x00000000;
    int TEXT_COLOR = 0x00FF00FF;
    int FOCUSED_TEXT_COLOR = 0x000000FF;
    int FOCUSED_BASE_COLOR = 0x00CC00FF;
    int CLICKED_BASE_COLOR = 0x008000FF;
};

class UiManager {
   private:
    bool showExitMessage;
    int menuWidth;
    int menuHeight;
    int gap;
    void initStyles();
    void changeGameState(GameState state);
   public:
    GameState state;
    void drawMenu();
    void drawUi(World& world, EntityId player);
    void drawPauseMenu();
    void drawSettings(GameState previousState);
    void updateSizes();

    UiManager();
};
