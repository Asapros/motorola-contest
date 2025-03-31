#pragma once

#include "common.hpp"
#include "world.hpp"
#include "raygui.h"
#include "leaderboard.hpp"

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
    const std::vector<int> speedValues = { 0, 20, 40, 60, 80, 100, 120 };
    const std::vector<int> RPMValues = { 0, 1, 2, 3, 4, 5, 6 };
    const float radius = 80;
    const float RPMMulitplier = 1000;
    bool showExitMessage;
    const Vector2 speedDialPositionOffset = {100, 100};
    const Vector2 RPMDialPositionOffset = {300, 100};
    int menuWidth;
    int menuHeight;
    int gap;
    bool show_debug;
    void initStyles();
    void changeGameState(GameState state);
   public:
    GameState state;
    bool isFullscreen;
    void drawMenu();
    void drawLeaderboard(const std::vector<PlayerInfo>& players);
    void drawUi(World* world, EntityId player);
    void drawPauseMenu();
    void drawSettings(GameState previousState);
    void updateSizes();
    void drawSpeedometer(float value, float gear);
    void drawRPMmeter(float value);

    UiManager();
};
