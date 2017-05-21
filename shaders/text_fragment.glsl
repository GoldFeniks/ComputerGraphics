#version 440 core

in vec2 texture_pos;
out vec4 final_color;

uniform bool has_diff_texture;
uniform sampler2D diff;
uniform vec4 texture_color;


void main() {
    final_color = texture_color * vec4(1, 1, 1, texture(diff, texture_pos).r);
}