#version 450 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec3 color;

out vec3 color_v;

void main() {
    gl_Position = vec4(position, 0.0f, 1.0f);
    color_v     = color;
}
