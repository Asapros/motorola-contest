#include <memory>

#include <raylib.h>

#include "rigidbody.hpp"
#include "world.hpp"

void rigidbody_test() {
    InitWindow(100, 100, "rigidbody_test");
    ModelManager modelManager = ModelManager();
    std::shared_ptr<ModelWrapper> model = modelManager.getModel("assets/part.obj");
    std::shared_ptr<Rigidbody> rb =
        std::make_shared<Rigidbody>(model, Vector3{0, 0, 0}, 1.0);
    World world;
    world.spawnEntity(rb);
    Camera camera = {{20.0f, 20.0f, 20.0f},
                     {0.0f, 0.0f, 0.0f},
                     {0.0f, 1.0f, 0.0f},
                     20.0f,
                     CAMERA_ORTHOGRAPHIC};

    for (uint32_t i = 0; i < 100; i++) {
        world.update(GetFrameTime());

        BeginDrawing();
        BeginMode3D(camera);
        world.draw();
        EndMode3D();
        EndDrawing();
    }
    CloseWindow();
}
