#version 450 core

layout(location = 1) in vec2 fi_texcoord;

layout(location = 0) out vec4 fo_color;

uniform sampler2D u_texA;

void main() {
    vec4 color = texture(u_texA, fi_texcoord);
    //    fo_color = vec4(1.0, 1.0, color.b, 1.0);
    fo_color = color;
}