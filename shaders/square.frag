#version 450 core

uniform vec3 ui_color;

out vec4 fo_color;

void main() {
    fo_color = vec4(ui_color, 1.0);
}
