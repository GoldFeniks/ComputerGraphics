#version 440 core

uniform uint time;
uniform float scale;
uniform float size;
uniform uint type;
uniform uint period;

in vec3 proc_color;
in vec3 vertex_pos;
in vec2 texture_pos;

out vec4 final_color;

float pi = 3.141592653589793;

vec4 cond_color(bool cond) {
    if (cond) 
        return vec4(1);
    else 
        return vec4(0, 0, 0, 1);
}

vec4 wave(vec2 pos, int num) {
    vec3 result = vec3(0);
    uint offset = period / num;
    for (int i = 0; i < num; ++i) {
        float r = ((time + offset * i) % period) / float(period);
        float d = distance(texture_pos, pos);
        result += vec3(1) * sin(pi / 2 * (1 - clamp(abs(d - r), 0, 1.0 / num / 3) * num * 3));
    }
    return vec4(result, 1);
}

void main() {
    final_color = wave(vec2(0.5, 0.5), int(size));
}