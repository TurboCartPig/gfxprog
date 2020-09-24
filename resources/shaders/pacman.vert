#version 450 core

layout(location = 0) in vec2 vi_position;

uniform mat4 u_transform;
uniform mat4 u_view;
uniform mat4 u_projection;

void main() {
    gl_Position = u_projection * u_view * u_transform * vec4(vi_position, 0.0, 1.0);
}
