#version 450 core

layout(location = 0) in vec2 vi_position;

void main() {
    gl_Position = vec4(vi_position, 0.0, 1.0);
}