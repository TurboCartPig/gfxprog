#version 450 core

in vec3 v_frag_pos;
in vec3 v_normal;
in vec2 v_texcoord;
in vec3 v_view_pos;

out vec4 frag_color;

uniform vec4 u_model_color;

void main() {
    frag_color = u_model_color;
}
