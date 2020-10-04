#version 450 core

layout(location = 0) in vec2 vi_position;
layout(location = 1) in vec2 vi_texcoord;
layout(location = 2) in vec4 vi_rgba;
layout(location = 3) in uint vi_variant;

layout(location = 1) out vec2 vo_texcoord;
layout(location = 2) out vec4 vo_rgba;
layout(location = 3) flat out uint vo_variant;

uniform mat4 u_view;
uniform mat4 u_projection;

void main() {
    vo_texcoord = vi_texcoord;
    vo_rgba = vi_rgba;
    vo_variant = vi_variant;
    gl_Position = u_projection * u_view * vec4(vi_position, 0.0, 1.0);
}
