#version 450 core

in vec3 v_frag_pos;
in vec4 v_frag_pos_light_space;
in vec3 v_normal;
in vec2 v_texcoord;
in vec3 v_view_pos;

out vec4 frag_color;

uniform sampler2D u_diffuse_map;
uniform sampler2D u_shadow_map;
uniform vec4 u_model_color;
uniform struct DirectionalLight {
    vec3 color;
    vec3 direction;
    float specularity;
} u_directional_light;

/*
 * Calculates if the fragment is in shadow or not.
 */
float compute_shadow() {
    // Where should we sample the shadow map?
    vec3 proj_coords = v_frag_pos_light_space.xyz / v_frag_pos_light_space.w;
    proj_coords = proj_coords * 0.5 + 0.5;
    float current_depth = proj_coords.z;

    // Angle base bias
     float bias = max(0.005 * (1.0 - dot(v_normal, u_directional_light.direction)), 0.0005);

    float shadow = 0.0;
    vec2 texel_size = 1.0 / textureSize(u_shadow_map, 0);

    // Percentage closer filtering
    // Based on brute force implementation from [GPU Gems](https://developer.nvidia.com/gpugems/gpugems/part-ii-lighting-and-shadows/chapter-11-shadow-map-antialiasing)
    for (float x = -2.5; x <= 2.5; x += 0.5) {
        for (float y = -2.5; y <= 2.5; y += 0.5) {
            float pcf_depth = texture(u_shadow_map, proj_coords.xy + vec2(x, y) * texel_size).r;
            shadow += current_depth - bias > pcf_depth ? 1.0 : 0.0;
        }
    }
    shadow /= 144.0;

    return shadow;
}

/*
 * Compute the directional light's color contribution to the fragment color.
 * Additionaly takes "u_directional_light" as input.
 */
vec3 compute_directional_light() {
    // Ambient lighting
    float ambient_strength = 0.10;
    vec3 ambient = ambient_strength * u_directional_light.color;

    // Diffuse lighting
    vec3 light_dir = u_directional_light.direction;
    vec3 diffuse = max(dot(v_normal, light_dir), 0.0) * u_directional_light.color;

    // Specular lighting
    vec3 view_dir = normalize(v_frag_pos - v_view_pos);
    vec3 reflected_dir = reflect(light_dir, v_normal);

    float spec = pow(max(dot(view_dir, reflected_dir), 0.0), 64);
    vec3 specular = u_directional_light.specularity * spec * u_directional_light.color;

    float shadow = compute_shadow();

    return (ambient + (1.0 - shadow) * (diffuse + specular));
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

    vec3 lighting = color * base_color.rgb;
    frag_color = vec4(lighting, 1.0);
}
