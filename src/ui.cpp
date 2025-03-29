#include <format>

#include "raylib.h"
#include "ui.hpp"
#include "vehicle.hpp"

UiManager::UiManager() {
    updateSizes();
    initStyles();
    changeGameState(GameState::MainMenu);
}

void UiManager::drawMenu() {
    if (IsKeyPressed(KEY_N))
        changeGameState(GameState::InGame);

    DrawText("RAYDER", 20, 20, 20, WHITE);
}

void UiManager::drawUi([[maybe_unused]] World& world,
                       [[maybe_unused]] EntityId playerId) {
    DrawText("ui", 0, 0, 20, RED);
}

void UiManager::drawSettings(GameState previousState) {
    changeGameState(GameState::InSettings);
    DrawText("RAYDER SETTINGS", 20, 20, 20, WHITE);
}

void UiManager::drawPauseMenu() {
    changeGameState(GameState::InPause);
    DrawText("RAYDER PAUSE", 20, 20, 20, WHITE);
}

void UiManager::updateSizes() {
    menuWidth = GetScreenWidth();
    menuHeight = GetScreenHeight();
    gap = menuHeight / 5;
}

void UiManager::initStyles() {
    Styles style;
    GuiSetStyle(DEFAULT, TEXT_SIZE, GetScreenHeight() / 30);

    GuiSetStyle(BUTTON, BORDER_COLOR_FOCUSED, style.BORDER_COLOR);
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, style.FOCUSED_BASE_COLOR);
    GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, style.FOCUSED_TEXT_COLOR);

    GuiSetStyle(BUTTON, BORDER_COLOR_PRESSED, style.BORDER_COLOR);
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, style.CLICKED_BASE_COLOR);
    GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, style.FOCUSED_TEXT_COLOR);

    GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, style.BORDER_COLOR);
    GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, style.BASE_COLOR);
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, style.TEXT_COLOR);
    GuiSetStyle(DEFAULT, BACKGROUND_COLOR, style.BASE_COLOR);
}

void UiManager::changeGameState(GameState state) {
    this->state = state;
}