#pragma once

#include "common.hpp"
#include "world.hpp"

class UiManager {
   public:
    void drawMenu();
    void drawUi(World& world, EntityId player);

    UiManager();
};
