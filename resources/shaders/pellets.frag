#version 450 core

in vec3 v_frag_pos;
in vec3 v_normal;
in vec2 v_texcoord;
in vec3 v_view_pos;

out vec4 frag_color;

void main() {
	frag_color = vec4(1.0, 1.0, 0.0, 1.0);
}