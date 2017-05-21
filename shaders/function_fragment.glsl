#version 440 compatibility

in vec3 proc_color;
in vec3 vertex_pos;
in vec3 normal;
in vec2 texture_pos;
in mat3 TBN;
out vec4 final_color;

uniform vec3 camera_pos;
uniform int light_size;
uniform sampler2D diff;
uniform sampler2D spec;
uniform sampler2D norm;
uniform bool has_diff_texture;
uniform bool has_spec_texture;
uniform bool has_norm_texture;
uniform float shininess;
uniform uint time;
uniform float scale;
uniform float size;

float pi = 3.141592653589793;

vec2 nearest_node(vec2 vec, float num) {
    return vec2(round(vec.x / num), round(vec.y / num));
}

vec2 bottom_left_node(vec2 vec, float num) {
    return vec2(floor(vec.x / num), floor(vec.y / num));
}

vec4 squares(int anim_style) {
    float nsize = size;
    vec4 c1 = vec4(1), c2 = vec4(0, 0, 0, 1);
    if (anim_style == 0)
        nsize = max(0.1, size * sin(pi * (time % 5000) / 5000));
    if (anim_style == 1) {
        float color1 = sin(pi * (time % 5000) / 5000);
        float color2 = sin(pi * ((time + 2500) % 5000) / 5000);
        uint t = time % 15000;
        if (time % 1000 < 500) {
            c1 = vec4(color1, 0, 0, 1);//color1 * int(t >= 5000 && t < 10000), color1 * int(t > 10000), 1);
            c2 = vec4(0, 0, color2, 1);
        }
        else {
            c2 = vec4(color1, 0, 0, 1);//color1 * int(t >= 5000 && t < 10000), color1 * int(t > 10000), 1);
            c1 = vec4(0, 0, color2, 1);
        }
    }
    vec2 node = bottom_left_node(texture_pos * scale, nsize);
    if (int(node.x) % 2 == int(node.y) % 2)
        return c1;
    else
        return c2;
}

vec4 wave(vec2 pos, int num) {
    vec3 result = vec3(0);
    int offset = 5000 / num;
    for (int i = 0; i < num; ++i) {
        float r = ((time + offset * i) % 5000) / 5000.0;
        float d = distance(texture_pos, pos);
        result += vec3(1) * sin(pi / 2 * (1 - clamp(abs(d - r), 0, 1.0 / num / 3) * num * 3));
    }
    return vec4(result, 1);
}

// vec4 waves() {
//     vec2 d = camera_pos.xy - vertex_pos.xy;
//     floor d1 = (time % 5000) / 5000;
//     return vec4(1) * sin(pi / 2 * (1 - clamp(length(d), 0, 1)) * d1);
// }

void main() {
    float scale = 100;
    float nx = texture_pos.x * scale, ny = texture_pos.y * scale;
    int inx = int(nx), iny = int(ny);
    float px = round(nx / 10) * 10, py = round(ny / 10) * 10;
    float x = nx - px;
    float y = ny - py;
    //if (x * x + y * y <= 16 * (sin(pi / 2 * (0.5 - abs(0.5 - texture_pos.x))) + sin(pi / 2 * (0.5 - abs(0.5 - texture_pos.y)))))
    int t = int(time);
    //if ((x * x + y * y <= 16 * sin(pi * (t % 5000) / 5000)) && ((t % 50000) >= ((px / 10) - 1) * 5000) && ((t % 50000) <= (px / 10) * 5000))
        //final_color = vec4(1, 0, 0, 1);
    //else
        //final_color = vec4(0, 0, 1, 1);
    float pos = sin(pi * (t % 5000) / 5000);
    //final_color = vec4(1) * sin(pi / 2 * (0.5 - pos + texture_pos.x));
    final_color = wave(vec2(0.5, 0.5), 10) + wave(vec2(0, 0.5), 10) + wave(vec2(1, 0.5), 10);// + wave(vec2(1, 1), 10);//wave(-1000) + wave(-750) + wave(-500) + wave(-250) + wave(0) + wave(250) + wave(500) + wave(750) + wave(1000);
}