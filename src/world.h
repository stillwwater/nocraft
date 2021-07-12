#ifndef WORLD_H
#define WORLD_H

#include <vector>

#include "chunk.h"
#include "rendering.h"
#include "camera.h"

class World {
public:
    Vector3 player_position = Vector3(0.0f, 20.0f, 0.0f);
    Camera camera{Vector3(0.0f, 20.0f, 0.0f)};
    Renderer renderer;

    std::vector<Chunk *> chunks;

    World(const World &) = delete;
    World& operator=(const World&) = delete;

    ~World() = default;

    void load();
};

#endif // WORLD_H
