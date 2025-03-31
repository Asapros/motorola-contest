#include <cstdlib>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

#include "collidable.hpp"
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

class Game;

#include "debug.hpp"
#include "entity.hpp"
#include "game.hpp"
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
    this->model = LoadModel(model_path.c_str());
}
ModelWrapper::~ModelWrapper() {
    UnloadModel(this->model);
}

ModelManager::ModelManager() {
    exe_location = get_executable_location();
    std::cerr << "get_executable_location() = " << exe_location.string()
              << '\n';
    std::filesystem::path assets_tmp = exe_location;
    assets_tmp.append("assets");
    if (std::filesystem::is_directory(assets_tmp)) {
        assets_location = assets_tmp;
    } else {
        assets_tmp = exe_location;
        assets_tmp.append("../assets");
        if (std::filesystem::is_directory(assets_tmp)) {
            assets_location = assets_tmp;
        }
    }
    std::cerr << "assets_location = " << assets_location << '\n';

    if (assets_location.empty()) {
        std::cerr << "Assets directory not found!\n";
        std::exit(1);
    }
}

std::shared_ptr<ModelWrapper> ModelManager::getModel(std::string model_path) {
    if (models.count(model_path)) {
        std::weak_ptr<ModelWrapper> mw_weak = models[model_path];
        if (!mw_weak.expired()) {
            return mw_weak.lock();
        }
    }

    std::filesystem::path resolved_path = assets_location;
    resolved_path.append(model_path);
    std::cerr << "resolved_path = " << resolved_path << '\n';

    std::shared_ptr<ModelWrapper> new_model =
        std::make_shared<ModelWrapper>(resolved_path.string());
    models[model_path] = new_model;
    return new_model;
}

void ModelManager::loadMap(std::string map_path, Game* game) {
    debugLog("MENU", std::format("loading map '{}'", map_path));

    std::filesystem::path map_entities_path = assets_location;
    map_entities_path.append(map_path);
    map_entities_path.concat("_entities.txt");

    std::ifstream entities_ifs(map_entities_path);
    if (!entities_ifs.is_open()) {
        std::cerr << "Couldn\'t load map!\n";
        std::exit(1);
    }

    std::map<int, std::vector<Vector2>> colliders;

    std::string line_buf;
    while (std::getline(entities_ifs, line_buf)) {
        std::stringstream ss(line_buf);
        std::string cmd;
        ss >> cmd;

        if (cmd == "entity") {
            std::string model;
            Vector3 position;
            float heading;
            std::optional<int> collider;

            while (ss) {
                std::string param_name;
                ss >> param_name;
                if (param_name == "model") {
                    ss >> model;
                } else if (param_name == "position") {
                    ss >> position.x >> position.y >> position.z;
                } else if (param_name == "heading") {
                    ss >> heading;
                } else if (param_name == "collider") {
                    int collider_tmp;
                    ss >> collider_tmp;
                    collider = collider_tmp;
                }
            }

            if (collider) {
                std::shared_ptr<Collidable> ent = std::make_shared<Collidable>(
                    getModel(model), position, heading,
                    colliders[collider.value()]);
                game->world->spawnEntity(ent);
            } else {
                std::shared_ptr<Entity> ent = std::make_shared<Entity>(
                    getModel(model), position, heading);
                game->world->spawnEntity(ent);
            }
        } else if (cmd == "collider") {
            int collider_id, verts_count;
            ss >> collider_id >> verts_count;
            colliders.insert({collider_id, {}});
            for (int i = 0; i < verts_count; i++) {
                float x, y;
                ss >> x >> y;
                colliders[collider_id].emplace_back(x, y);
            }
        }
    }

    std::filesystem::path map_materials_path = assets_location;
    map_materials_path.append(map_path);
    map_materials_path.concat("_materials.txt");

    std::ifstream materials_ifs(map_materials_path);
    if (!materials_ifs.is_open()) {
        std::cerr << "Couldn\'t load map!\n";
        std::exit(1);
    }

    int grid_size;
    materials_ifs >> grid_size;
    game->world->materials.resize(grid_size);
    for (int i = 0; i < grid_size; i++) {
        game->world->materials[i].resize(grid_size);
    }
    for (int i = 0; i < grid_size; i++) {
        for (int j = 0; j < grid_size; j++) {
            materials_ifs >> game->world->materials[j][i];
        }
    }
}
