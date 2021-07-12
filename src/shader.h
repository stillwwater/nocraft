#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glad/glad.h>

#include "xmath.h"

struct Shader {
    enum Type {
        Unlit,
    };

    GLuint id;

    Shader() = default;
    Shader(GLuint program) : id{ program } {}

    GLuint uniform_location(const char* name) const;

    void uniform(const char *name, float value) const;
    void uniform(const char *name, int value) const;
    void uniform(const char *name, const Vector2 &value) const;
    void uniform(const char *name, const Vector3 &value) const;
    void uniform(const char *name, const Vector4 &value) const;
    void uniform(const char *name, const Matrix4 &value) const;
};

Shader load_shader_program(const std::string &vert_name, const std::string &frag_name);

inline void Shader::uniform(const char *name, float value) const {
    glUniform1f(uniform_location(name), value);
}

inline void Shader::uniform(const char *name, int value) const {
    glUniform1i(uniform_location(name), value);
}

inline void Shader::uniform(const char *name, const Vector2 &v) const {
    glUniform2fv(uniform_location(name), 1, reinterpret_cast<const float *>(&v));
}

inline void Shader::uniform(const char *name, const Vector3 &v) const {
    glUniform3fv(uniform_location(name), 1, reinterpret_cast<const float *>(&v));
}

inline void Shader::uniform(const char *name, const Vector4 &v) const {
    glUniform4fv(uniform_location(name), 1, reinterpret_cast<const float *>(&v));
}

inline void Shader::uniform(const char *name, const Matrix4 &m) const {
    glUniformMatrix4fv(uniform_location(name), 1, GL_FALSE, reinterpret_cast<const float *>(&m));
}

#endif // SHADER_H
