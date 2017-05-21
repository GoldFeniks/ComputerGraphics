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

vec4 lines() {
    vec2 pos = texture_pos * scale;
    float stroke = scale / size / 100.0;
    vec2 node = round(pos / size) * size;
    if (type == 0)
        return cond_color((abs(pos.x - node.x) <= stroke) || (abs(pos.y - node.y) <= stroke));
    else if (type == 1)
        return cond_color(abs(sin(pos.x) - sin(pos.y)) <= stroke);
    else if (type == 2)
        return cond_color((abs(sin(pos.x) - pos.y + node.y) <= stroke) || (abs(sin(pos.y) - pos.x + node.x) <= stroke));
    else if (type >= 3) {
        float a = 2 * pi  * (time % period) / period; 
        if (type == 3)
            return cond_color((abs(sin(pos.x + a + pi * sin(a)) - pos.y + node.y) <= stroke) || (abs(sin(pos.y + a + pi * sin(a)) - pos.x + node.x) <= stroke)); 
        if (type == 4)
            return cond_color((abs(sin(pos.x + a) * sin(pos.x) - pos.y + node.y) <= stroke) || (abs(sin(pos.y + a) * sin(pos.y) - pos.x + node.x) <= stroke)); 
        if (type == 5)
            return cond_color((abs(sin(pos.x + a) * sin(pos.x - a) - pos.y + node.y) <= stroke) || (abs(sin(pos.y + a) * sin(pos.y - a) - pos.x + node.x) <= stroke)); 
        if (type == 6)
            return cond_color((abs(sin(pos.x + a) * sin(a) - pos.y + node.y) <= stroke) || (abs(sin(pos.y + a) * sin(a) - pos.x + node.x) <= stroke)); 
        if (type == 7)
            return cond_color((abs(sin(pos.x) * sin(a) - pos.y + node.y) <= stroke) || (abs(sin(pos.y) * sin(a) - pos.x + node.x) <= stroke));
    }
    return vec4(1);
}

void main() {
    final_color = lines();
}