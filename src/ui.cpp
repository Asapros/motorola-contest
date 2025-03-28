#include "raylib.h"
#include "ui.hpp"
#include <format>
#include "vehicle.hpp"

UiManager::UiManager() {}

void UiManager::drawMenu() {
    DrawText("Main menu", 0, 0, 20, WHITE);
    DrawText("Press space to play", 20, 60, 16, WHITE);
}

void UiManager::drawUi(World& world, EntityId playerId) {
    DrawText("ui", 0, 0, 20, RED);
}