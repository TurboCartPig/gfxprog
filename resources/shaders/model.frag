#version 450 core

in vec3 v_frag_pos;
in vec3 v_normal;
in vec2 v_texcoord;
in vec3 v_view_pos;

out vec4 frag_color;

uniform sampler2D u_diffuse_map;
uniform vec4 u_model_color;
uniform struct DirectionalLight {
    vec3 color;
    vec3 direction;
    float specularity;
} u_directional_light;

/*
 * Compute the directional light's color contribution to the fragment color.
 * Additionaly takes "u_directional_light" as input.
 */
vec3 compute_directional_light() {
    // Ambient lighting
    float ambient_strength = 0.10;
    vec3 ambient = ambient_strength * u_directional_light.color;

    // Diffuse lighting
    vec3 light_dir = normalize(u_directional_light.direction);
    vec3 diffuse = max(dot(v_normal, light_dir), 0.0) * u_directional_light.color;

    // Specular lighting
    vec3 view_dir = normalize(v_frag_pos - v_view_pos);
    vec3 reflected_dir = reflect(light_dir, v_normal);

    float spec = pow(max(dot(view_dir, reflected_dir), 0.0), 32);
    vec3 specular = u_directional_light.specularity * spec * u_directional_light.color;

    return ambient + diffuse + specular;
}

/*
 * Compute a point light's color contribution to the fragment color.
 */
vec3 compute_point_light() {
    return vec3(0.0, 0.0, 0.0);
}

void main() {
    vec4 base_color;
    if (u_model_color.a == 0.0) {
        base_color = texture(u_diffuse_map, v_texcoord);
    } else {
        base_color = u_model_color;
    }

    vec3 color = vec3(0.0, 0.0, 0.0);

    color += compute_directional_light();
    color += compute_point_light();

    frag_color = base_color * vec4(color, 1.0);
}
