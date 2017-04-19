#version 440 compatibility

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 in_color;
layout(location = 2) in vec3 vertex_normal;
layout(location = 3) in vec2 texture_position;
layout(location = 4) in vec3 vertex_tangent;
layout(location = 5) in vec3 vertex_bitangent;

out vec3 proc_color;
out vec3 vertex_pos;
out vec3 normal;
out vec2 texture_pos;
out mat3 TBN;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
    gl_PointSize = 10;
    gl_Position = projection * view * model * vec4(position, 1);
    vertex_pos = vec3(model * vec4(position, 1));
    mat3 normal_matrix = mat3(transpose(inverse(model)));
    normal = normal_matrix * vertex_normal;
    texture_pos = texture_position;
    vec3 T = normalize(normal_matrix * vertex_tangent);
    vec3 B = normalize(normal_matrix * vertex_bitangent);
    vec3 N = normalize(normal_matrix * vertex_normal);
    T = normalize(T - dot(T, N) * N);
    if (dot(cross(N, T), B) < 0) {
        T = T * -1;
    }
    TBN = transpose(mat3(T, B, N));
}