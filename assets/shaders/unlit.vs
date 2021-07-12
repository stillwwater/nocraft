#version 330 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec4 VColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 Color;

void main() {
    Color = VColor;
    gl_Position = projection * view * model * vec4(Position, 1.0);
}
