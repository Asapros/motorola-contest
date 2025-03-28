#pragma once

#include "world.hpp"
#include "common.hpp"

class UiManager {
   public:
    void drawMenu();
    void drawUi(World& world, EntityId player);

    UiManager();
};