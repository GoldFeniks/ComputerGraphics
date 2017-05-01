#version 440 core
layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texture_position;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 texture_pos;

void main() {
    gl_Position = projection * model * vec4(position.xy, 0, 1);
    texture_pos = texture_position;
}