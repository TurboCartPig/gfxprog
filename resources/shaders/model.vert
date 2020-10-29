#version 450 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texcoord;

out vec3 v_frag_pos;
out vec3 v_normal;
out vec2 v_texcoord;
out vec3 v_view_pos;

//uniform mat4 u_projection;
//uniform mat4 u_view;
uniform mat4 u_transform;
uniform mat4 u_view_projection;

void main() {
    v_frag_pos = a_position;

    mat3 norm_mat = transpose(inverse(mat3(u_transform)));
    v_normal = norm_mat * normalize(a_normal);

    v_texcoord = a_texcoord;

    //    v_view_pos = u_view[3].xyz;
    v_view_pos = u_view_projection[3].xyz;

    //    gl_Position = u_projection * u_view * u_transform * vec4(v_frag_pos, 1.0);
    gl_Position = u_view_projection * u_transform * vec4(v_frag_pos, 1.0);
}
