#ifndef RENDERING_H
#define RENDERING_H

#include "shader.h"
#include "camera.h"
#include "chunk.h"

class World;

class Renderer {
public:
    Vector4 clearcolor{.627f, .866f, .952f, 1.f};

    Renderer() = default;

    void gen_buffers();
    void load_shaders();

    void draw(World *world);

private:
    Shader shaders[64];
};

#endif // RENDERING_H
