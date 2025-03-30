#include <format>
#include <vector>

#include "raylib.h"
#include "raymath.h"

#include "debug.hpp"
#include "ui.hpp"
#include "vehicle.hpp"

UiManager::UiManager() {
    updateSizes();
    initStyles();
    changeGameState(GameState::MainMenu);
    showExitMessage = false;
}

void UiManager::drawMenu() {
    if (IsKeyPressed(KEY_N))
        changeGameState(GameState::InGame);

    DrawText("RAYDER", 20, 20, 20, WHITE);

    Rectangle playButton = {(float)(menuWidth / 2 - 100),
                            (float)(menuHeight / 2 - gap), 200.0f, 80.0f};
    Rectangle settingsButton = {(float)(menuWidth / 2 - 100),
                                (float)(menuHeight / 2), 200.0f, 80.0f};
    Rectangle exitButton = {(float)(menuWidth / 2 - 100),
                            (float)(menuHeight / 2 + gap), 200.0f, 80.0f};

    if (GuiButton(playButton, "PLAY") && !showExitMessage) {
        changeGameState(GameState::InGame);
    }

    if (GuiButton(settingsButton, "SETTINGS") && !showExitMessage) {
        changeGameState(GameState::InSettings);
    }

    if (GuiButton(exitButton, "EXIT") && !showExitMessage) {
        showExitMessage = true;
    }

    if (showExitMessage) {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
        int result =
            GuiMessageBox({(float)GetScreenWidth() / 2 - 275,
                           (float)GetScreenHeight() / 2 - 125, 550, 250},
                          GuiIconText(ICON_EXIT, "Close Window"),
                          "Do you really want to exit?", "Yes;No");

        if ((result == 0) || (result == 2))
            showExitMessage = false;
        else if (result == 1)
            exit(0);
    }
}

void UiManager::drawUi([[maybe_unused]] World& world,
                       [[maybe_unused]] EntityId playerId) {}

void UiManager::drawMeter(float value,
                          float multiplier,
                          float second_value,
                          Vector2 position,
                          float radius,
                          const char* unit,
                          std::vector<int> values) {
    if (values.empty()) {
        debugLog("GENERAL", "Empty meter's values list");
        return;
    }

    int numValues = values.size();
    int v_min = 0;
    int v_max = values[numValues - 1];
    float interpolatedValue = Lerp((float)v_min, (float)v_max, value);

    if (multiplier != 1) {
        if (value / multiplier > v_max)
            value = v_max * multiplier;
    }

    DrawCircleLines(position.x, position.y, radius, WHITE);

    if (second_value != 0.0f && unit != NULL) {
        char smallText[20];
        sprintf(smallText, "%d %s", (int)second_value, unit);
        int textWidth = MeasureText(smallText, GetScreenHeight() / 30);
        DrawText(smallText, position.x - textWidth / 2, position.y + 30,
                 GetScreenHeight() / 30, WHITE);
    } else if (second_value != 0.0f && unit == NULL) {
        char smallText[20];
        sprintf(smallText, "%d", (int)second_value);
        int textWidth = MeasureText(smallText, GetScreenHeight() / 30);
        DrawText(smallText, position.x - textWidth / 2, position.y + 30,
                 GetScreenHeight() / 30, WHITE);
    } else if (second_value == 0.0f && unit != NULL) {
        char smallText[20];
        sprintf(smallText, "%s", unit);
        int textWidth = MeasureText(smallText, GetScreenHeight() / 30);
        DrawText(smallText, position.x - textWidth / 2, position.y + 30,
                 GetScreenHeight() / 30, WHITE);
    }

    float startAngle = -200;
    float endAngle = 20;
    float fullAngle = abs(startAngle) + abs(endAngle);
    float needleAngle = fullAngle * interpolatedValue;
    float angleStep = (endAngle - startAngle) / (numValues - 1);

    for (int i = 0; i < numValues; i++) {
        float angle = startAngle + i * angleStep;
        float radian = angle * DEG2RAD;

        int textX = (int)((float)position.x + cosf(radian) * (radius - 25.0f));
        int textY =
            (int)((float)position.y + sinf(radian) * (radius - 25) - 10);

        char valText[5];
        sprintf(valText, "%d", values[i]);

        int textWidth = MeasureText(valText, 20);
        DrawText(valText, textX - textWidth / 2, textY, 20, WHITE);
    }

    if (value / multiplier <= values[numValues - 1]) {
        for (int i = 0; i < numValues - 1; i++) {
            if (value / multiplier >= values[i] &&
                value / multiplier <= values[i + 1]) {
                float t = (value / multiplier - values[i]) /
                          (float)(values[i + 1] - values[i]);
                needleAngle = startAngle + i * angleStep + t * angleStep;
                break;
            }
        }
    } else {
        float extraSpeed = value - values[numValues - 1];
        needleAngle = endAngle + (extraSpeed / 20.0f) * angleStep;
    }

    float needleRadian = needleAngle * DEG2RAD;
    float needleLength = radius - 20.0f;
    Vector2 needleEnd = {position.x + cosf(needleRadian) * needleLength,
                         position.y + sinf(needleRadian) * needleLength};

    DrawLineEx(Vector2{(float)position.x, (float)position.y}, needleEnd, 5,
               RED);
}

void UiManager::drawSettings(GameState previousState) {
    changeGameState(GameState::InSettings);
    DrawText("RAYDER SETTINGS", 20, 20, 20, WHITE);

    Rectangle fullscreenCheckbox = {(float)(menuWidth / 2 - 100),
                                    (float)(menuHeight / 2 - gap), 80.0f,
                                    80.0f};
    Rectangle applyButton = {(float)(menuWidth / 2 - 100),
                             (float)(menuHeight / 2 + gap), 200.0f, 80.0f};

    if (GuiButton(applyButton, "APPLY") || IsKeyPressed(KEY_ESCAPE)) {
        if (previousState == GameState::MainMenu ||
            previousState == GameState::InPause) {
            changeGameState(previousState);
        } else {
            changeGameState(GameState::InGame);
        }
    }
}

void UiManager::drawPauseMenu() {
    changeGameState(GameState::InPause);
    DrawText("RAYDER PAUSE", 20, 20, 20, WHITE);

    Rectangle playButton = {(float)(menuWidth / 2 - 100),
                            (float)(menuHeight / 2 - gap), 200.0f, 80.0f};
    Rectangle settingsButton = {(float)(menuWidth / 2 - 100),
                                (float)(menuHeight / 2), 200.0f, 80.0f};
    Rectangle returnButton = {(float)(menuWidth / 2 - 100),
                              (float)(menuHeight / 2 + gap), 200.0f, 80.0f};

    if (GuiButton(playButton, "CONTINUE")) {
        changeGameState(GameState::InGame);
    }

    if (GuiButton(settingsButton, "SETTINGS")) {
        changeGameState(GameState::InSettings);
    }

    if (GuiButton(returnButton, "EXIT TO MENU")) {
        changeGameState(GameState::MainMenu);
    }
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
