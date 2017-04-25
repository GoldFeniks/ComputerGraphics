#version 440 compatibility

out vec4 final_color;

void main() {
    final_color = gl_FrontMaterial.ambient;
}