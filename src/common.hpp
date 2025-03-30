#pragma once

using EntityId = unsigned int;

#define GRAVITY_ACCELERATION 10.0

enum class GameState {
    MainMenu,
    InGame,
    InPause,
    InSettings,
};
