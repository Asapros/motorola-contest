#pragma once

#include <filesystem>
#include <map>
#include <memory>
#include <string>

#include <raylib.h>

// class ModelManager;

// #include "game.hpp"

class Game;

class ModelWrapper {
   public:
    Model model;

    ModelWrapper(std::string model_path);
    ~ModelWrapper();
};

class ModelManager {
   private:
    std::map<std::string, std::weak_ptr<ModelWrapper>> models;
    std::filesystem::path exe_location;
    std::filesystem::path assets_location;

   public:
    ModelManager();
    std::shared_ptr<ModelWrapper> getModel(std::string model_path);
    void loadMap(std::string map_path, Game* game);
};
