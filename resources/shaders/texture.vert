#version 450 core

layout(location = 0) in vec2 vi_position;
layout(location = 1) in vec2 vi_texcoord;

layout(location = 1) out vec2 vo_texcoord;

void main() {
    vo_texcoord = vi_texcoord;
    gl_Position = vec4(vi_position, 0.0, 1.0);
}