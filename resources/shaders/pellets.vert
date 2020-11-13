#version 450 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texcoord;
layout(location = 4) in mat4 a_transform;

out vec3 v_frag_pos;
out vec4 v_frag_pos_light_space;
out vec3 v_normal;
out vec2 v_texcoord;
out vec3 v_view_pos;

uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_light_space_matrix;

void main() {
    v_frag_pos = a_position;

    v_frag_pos_light_space = u_light_space_matrix * a_transform * vec4(a_position, 1.0);

    mat3 norm_mat = mat3(transpose(inverse(a_transform)));
    v_normal = normalize(norm_mat * a_normal);

    v_texcoord = a_texcoord;

    v_view_pos = u_view[3].xyz;

    gl_Position = u_projection * u_view * a_transform * vec4(a_position, 1.0);
}