#include "rendering.h"

#include <glad/glad.h>
#include <cstdio>
#include <cstring>

#include "xmath.h"
#include "shader.h"
#include "chunk.h"
#include "camera.h"
#include "world.h"

void Renderer::gen_buffers() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);
}

void Renderer::load_shaders() {
    shaders[Shader::Unlit] =
        load_shader_program("assets/shaders/unlit.vs", "assets/shaders/unlit.fs");
}


void Renderer::draw(World *world) {
    auto &shader = shaders[Shader::Unlit];

    glClearColor(clearcolor.x, clearcolor.y, clearcolor.z, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shader.id);

    // Transforms
    auto view = world->camera.view_matrix();
    auto projection = math::perspective(math::radians(60.0f), 800.f/600.f, 0.1f, 100.0f);

    shader.uniform("view", view);
    shader.uniform("projection", projection);

    // Render
    for (auto *chunk : world->chunks) {
        glBindVertexArray(chunk->VAO);

        auto model = Matrix4(1);
        model = math::translate(model, chunk->world_position());
        shader.uniform("model", model);

        glBindBuffer(GL_ARRAY_BUFFER, chunk->VBO);
        glDrawArrays(GL_TRIANGLES, 0, chunk->mesh.vertices.size());
    }

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
