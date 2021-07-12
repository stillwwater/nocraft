#include "shader.h"

#include <string>
#include <optional>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <glad/glad.h>

static char *read_file_text(const std::string &filename) {
    auto *file = fopen(filename.c_str(), "rb");
    fseek(file, 0, SEEK_END);
    size_t fsize = ftell(file);
    fseek(file, 0, SEEK_SET);

    auto *text = new char[fsize + 1];
    text[fsize] = '\0';
    fread(text, 1, fsize, file);

    fclose(file);
    return text;
}

static std::optional<GLuint> load_shader_stage(int type, const std::string &filename) {
    auto shader = glCreateShader(type);
    auto *shader_src = read_file_text(filename);
    int ok;

    printf("[SHADER] compiling shader %s\n", filename.c_str());

    glShaderSource(shader, 1, &shader_src, nullptr);
    glCompileShader(shader);
    delete[] shader_src;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char info[512];
        glGetShaderInfoLog(shader, sizeof(info), nullptr, info);
        printf("[SHADER] error: failed to compile shader %s:\n%s\n", filename.c_str(), info);
        return {};
    }
    return shader;
}

Shader load_shader_program(const std::string &vert_name, const std::string &frag_name) {
    auto vert = load_shader_stage(GL_VERTEX_SHADER, vert_name);
    auto frag = load_shader_stage(GL_FRAGMENT_SHADER, frag_name);
    assert(vert && frag);

    auto id = glCreateProgram();
    glAttachShader(id, vert.value());
    glAttachShader(id, frag.value());
    glLinkProgram(id);

    int ok;
    glGetProgramiv(id, GL_LINK_STATUS, &ok);
    if (!ok) {
        char info[512];
        glGetProgramInfoLog(id, sizeof(info), nullptr, info);
        printf("[SHADER] error: failed to link shader program:\n%s\n", info);
    }
    assert(ok && "Failed to create shader program");

    glDeleteShader(vert.value());
    glDeleteShader(frag.value());
    return Shader{id};
}

GLuint Shader::uniform_location(const char *name) const {
    return glGetUniformLocation(id, name);
}
