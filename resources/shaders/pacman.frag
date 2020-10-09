#version 450 core

layout(location = 1) in vec2 fi_texcoord;// In normalized coordinates
layout(location = 2) in vec4 fi_rgba;
layout(location = 3) flat in float fi_variant;

out vec4 fo_color;

layout(location = 0) uniform sampler2DRect u_sprite_sheet;

void main() {
    if (fi_variant == 1) { // Textured sprites
        vec2 coord = mix(fi_rgba.xy, fi_rgba.zw, fi_texcoord);
        fo_color = texture(u_sprite_sheet, coord);
    } else if (fi_variant == 2) { // Pellets
        if (abs(length(fi_texcoord - vec2(0.5, 0.5))) > 0.25) {
            discard;
        } else {
            fo_color = fi_rgba;
        }
    } else { // Other
        fo_color = fi_rgba;
    }
}
