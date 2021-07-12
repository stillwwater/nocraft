#include "world.h"

#include <vector>

#include "rendering.h"
#include "chunk.h"
#include "xmath.h"

void World::load() {
    renderer.gen_buffers();
    renderer.load_shaders();

    for (int x = -6; x <= 6; ++x) {
        for (int z = -6; z <= 6; ++z) {
            chunks.push_back(load_chunk(Point3{x, 0, z}));
        }
    }
}
