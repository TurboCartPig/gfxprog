#version 450 core

uniform vec3 u_color;

out vec4 fo_color;

void main() {
    fo_color = vec4(u_color, 1.0);
}
