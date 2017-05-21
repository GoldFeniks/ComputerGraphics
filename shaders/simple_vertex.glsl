#version 440 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 in_color;
layout(location = 2) in vec2 texture_position;

out vec3 proc_color;
out vec3 vertex_pos;
out vec2 texture_pos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
    gl_Position = projection * view * model * vec4(position, 1);
    vertex_pos = vec3(model * vec4(position, 1));
    texture_pos = texture_position;
}