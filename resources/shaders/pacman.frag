#version 450 core

out vec4 fo_color;

uniform vec3 u_color;

void main() {
    fo_color = vec4(u_color, 1.0);
}
