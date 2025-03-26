#include "model_manager.hpp"
#include "raylib.h"
#include <memory>
#include "debug.hpp"
#include <format>


ModelWrapper::ModelWrapper(std::string model_path) {
    this->model = LoadModel(model_path.c_str());
}
ModelWrapper::~ModelWrapper() {
    UnloadModel(this->model);
}

ModelManager::ModelManager() {}

std::shared_ptr<ModelWrapper> ModelManager::getModel(std::string model_path) {
    if (models.count(model_path)) {
        std::weak_ptr<ModelWrapper> mw_weak = models[model_path];
        if (!mw_weak.expired()) {
            return mw_weak.lock();
        }
    }

    std::shared_ptr<ModelWrapper> new_model = std::make_shared<ModelWrapper>(model_path);
    models[model_path] = new_model;
    return new_model;
}
