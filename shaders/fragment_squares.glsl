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

vec4 squares() {
    vec2 pos = texture_pos * scale;
    vec2 node_order = vec2(0);
    if (type <= 3) {
        if (type == 0)
            node_order = floor(pos / size);
        if (type == 1)
            node_order = floor(vec2(pos.x - sin(pos.y), pos.y - sin(pos.x)) / size);
        if (type == 2)
            node_order = floor(vec2(pos.x - sin(pos.y), pos.y - sin(pos.x + pi)) / size);
        if (type == 3)
            node_order = round(pos / size);
    }
    if (type >= 4 && type < 6) {
        float a = 2 * pi  * (time % period) / period;  
        if (type == 4)
            node_order = floor(vec2(pos.x - sin(pos.y) * sin(a), pos.y - sin(pos.x) * sin(a)) / size);
        if (type == 5)
            node_order = floor(vec2(pos.x - sin(pos.y + a) * sin(pos.y - a), pos.y - sin(pos.x + a) * sin(pos.x - a)) / size);
    }
    if (type >= 6) {
        float s = float(time % period) / period * size;
        if (type == 6)
            node_order = floor((pos + s) / size);
        if (type == 7)
            node_order = floor(vec2(pos.x - sin(pos.y) + s, pos.y - sin(pos.x) + s) / size);
        if (type >= 8) {
            float a = 2 * pi  * (time % period) / period;  
            if (type == 8)
                node_order = floor(vec2(pos.x - sin(pos.y) * sin(a) + s, pos.y - sin(pos.x) * sin(a) + s) / size);
            if (type == 9)
                node_order = floor(vec2(pos.x - sin(pos.y + a) * sin(pos.y - a) + s, pos.y - sin(pos.x + a) * sin(pos.x - a) + s) / size);
        }   
    }
    return cond_color(int(node_order.x) % 2 == int(node_order.y) % 2);
}

void main() {
    final_color = squares();
}