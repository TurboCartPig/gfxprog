#version 450 core

layout(location = 1) in vec2 fi_texcoord;// In normalized coordinates
layout(location = 2) in vec4 fi_rgba;
layout(location = 3) flat in uint fi_variant;

out vec4 fo_color;

layout(location = 0) uniform sampler2DRect u_sprite_sheet;

void main() {
    if (fi_variant != 0) { // variant somehow gets fucked during vertex -> fragment transfer. Should be 1, is 1065353216
        vec2 coord = mix(fi_rgba.xy, fi_rgba.zw, fi_texcoord);
        fo_color = texture(u_sprite_sheet, coord);
    } else {
        fo_color = fi_rgba;
    }
}
