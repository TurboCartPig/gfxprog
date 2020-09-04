#version 450 core

in vec3 color_v;

out vec4 FragColor;

void main() {
    FragColor = vec4(color_v, 1.0f);
    // FragColor = vec4(1.0);
}
