#version 450 core

layout (location = 0) in vec3 a_position;

uniform mat4 u_transform;
uniform mat4 u_light_space_matrix;

void main() {
    gl_Position = u_light_space_matrix * u_transform * vec4(a_position, 1.0);
}