#version 450 core

layout(location = 1) in vec2 fi_texcoord;// In normalized coordinates

out vec4 fo_color;

uniform vec4 u_color;
uniform ivec4 u_key_frame;// Offset into the texture to access only the key frame. In Non-normalized coordinates

uniform sampler2DRect u_sprite_sheet;

void main() {
    if (u_color.a < 0.1) {
        vec2 coord = mix(u_key_frame.xy, u_key_frame.zw, fi_texcoord);
        fo_color = texture(u_sprite_sheet, coord);
    } else {
        fo_color = u_color;
    }
}
