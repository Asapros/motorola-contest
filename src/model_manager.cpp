#include <filesystem>
#include <format>
#include <iostream>
#include <memory>

#include "raylib.h"

#ifdef _WIN32
// It was quite difficult to make Windows.h and Raylib not have conflicts with
// each other...
#define NOGDI
#define NOUSER
// namespace windows {
#include <Windows.h>
// }
/*#include <libloaderapi.h>*/
/*extern int GetModuleFileNameA(void* hModule, char* lpFilename, int nSize);*/
#endif

#include "debug.hpp"
#include "model_manager.hpp"

std::filesystem::path get_executable_location() {
#ifdef _WIN32
    const int buf_size = 512;
    char* name_buf = new char[buf_size];
    int name_len = GetModuleFileNameA(nullptr, name_buf, buf_size);
    std::string name_str(name_buf, name_len);
    return std::filesystem::path(name_str).parent_path();
#elif __linux__
    return std::filesystem::canonical("/proc/self/exe").parent_path();
#endif
}

ModelWrapper::ModelWrapper(std::string model_path) {
    std::cerr << "get_executable_location() = "
              << get_executable_location().string() << '\n';
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

    std::shared_ptr<ModelWrapper> new_model =
        std::make_shared<ModelWrapper>(model_path);
    models[model_path] = new_model;
    return new_model;
}
