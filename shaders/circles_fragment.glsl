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

vec4 circles() {
    vec2 pos = texture_pos * scale;
    float r = scale / size / 4;
    vec2 node = vec2(0);
    if (type >= 0 && type < 4) {
        node = round(pos / size) * size;
        if (type == 1)
            r = r + sin(pos.x) * sin(pos.y);
        if (type == 2)
            r = r * sin(pos.x) * sin(pos.y);
        if (type == 3)
            r = r * sin(pos.x);
    }
    if (type >= 4 && type < 7) {
        float a = 2 * pi  * (time % period) / period;  
        node = round(pos / size) * size;
        if (type == 4)
            r = r + sin(pos.x + a) * sin(pos.y + a);
        if (type == 5) {
            pos = pos - node;
            pos.x *= sin(a);
            pos.y *= cos(a);
            pos = pos + node;
        }
        if (type == 6)
            r *= sin(a);
    }
    if (type >= 7) {
        float s = float(time % period) / period * size;
        if (type == 7)
            node = round((pos - s) / size) * size + s;
        if (type == 8) {
            if (int(floor((pos.y - size / 2) / size)) % 2 == 0) {
                node = round(vec2((pos.x - s), pos.y) / size) * size;
                node.x += s;
            }
            else {
                node = round(vec2((pos.x + s), pos.y) / size) * size;
                node.x -= s; 
            }
        }
    }
    pos = pos - node;
    return cond_color(pos.x * pos.x + pos.y * pos.y <= r * r);
}

void main() {
    final_color = circles();
}