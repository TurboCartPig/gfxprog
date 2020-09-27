#version 450 core

layout(location = 0) in vec2 vi_position;
layout(location = 1) in vec2 vi_texcoord;

layout(location = 1) out vec2 vo_texcoord;

uniform mat4 u_transform;
uniform mat4 u_view;
uniform mat4 u_projection;

void main() {
    vo_texcoord = vi_texcoord;
    gl_Position = u_projection * u_view * u_transform * vec4(vi_position, 0.0, 1.0);
}
