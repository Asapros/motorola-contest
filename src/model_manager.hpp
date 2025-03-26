#pragma once

#include <map>
#include <memory>
#include <string>

#include <raylib.h>

class ModelWrapper {
   public:
    Model model;

    ModelWrapper(std::string model_path);
    ~ModelWrapper();
};

class ModelManager {
   private:
    std::map<std::string, std::weak_ptr<ModelWrapper>> models;

   public:
    ModelManager();
    std::shared_ptr<ModelWrapper> getModel(std::string model_path);
};
