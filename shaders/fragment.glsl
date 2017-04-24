#version 440 compatibility

in vec3 proc_color;
in vec3 vertex_pos;
in vec3 normal;
in vec2 texture_pos;
in mat3 TBN;
out vec4 final_color;

uniform vec3 camera_pos;
uniform int light_count;
uniform sampler2D diff;
uniform sampler2D spec;
uniform sampler2D norm;
uniform bool has_diff_texture;
uniform bool has_spec_texture;
uniform bool has_norm_texture;
uniform float shininess;

vec4 texdiff = vec4(1);
vec4 texspec = vec4(1);

void main() {
    vec3 new_normal = normal;
    vec3 view_dir = normalize(camera_pos - vertex_pos);
    if (has_norm_texture) {
        new_normal = texture(norm, texture_pos).xyz;
        new_normal = normalize(new_normal * 2.0 - 1.0);   
        new_normal = normalize(TBN * new_normal);
    }
    if (has_diff_texture) {
        texdiff = vec4(vec3(texture(diff, texture_pos)), 0);
    }
    if (has_spec_texture) {
        texspec = vec4(vec3(texture(spec, texture_pos)), 0);
    }
    for (int i = 0; i < light_count; ++i) {
        if (length(gl_LightSource[i].position) == 0) {
            final_color += gl_FrontLightProduct[i].ambient * texdiff + gl_FrontLightProduct[i].ambient;
        } else {
            vec3 light_dir;
            float attenuation = 1;
            float intensity = 1;
            if (gl_LightSource[i].position.w == 0) {
                light_dir = normalize(gl_LightSource[i].position.xyz);
            } else {
                light_dir = normalize(gl_LightSource[i].position.xyz - vertex_pos); 
                float dist = length(gl_LightSource[i].position.xyz - vertex_pos);
                attenuation = 1.0 / (gl_LightSource[i].constantAttenuation + gl_LightSource[i].linearAttenuation * dist + gl_LightSource[i].quadraticAttenuation * dist * dist);
                if (length(gl_LightSource[i].spotDirection) != 0) {
                    float theta = dot(light_dir, normalize(-gl_LightSource[i].spotDirection));
                    if (theta < gl_LightSource[i].spotCosCutoff) {
                        intensity = 0;
                    } else {
                        intensity = pow(max(theta, 0), gl_LightSource[i].spotExponent);
                    }
                }
            }
            vec3 reflect_dir = normalize(reflect(-light_dir, new_normal));
            vec4 ambient = gl_FrontLightProduct[i].ambient;
            vec4 diffuse = gl_FrontLightProduct[i].diffuse * max(dot(new_normal, light_dir), 0.0) * attenuation * intensity;
            vec4 specular = gl_FrontLightProduct[i].specular * pow(max(dot(view_dir, reflect_dir), 0.0), shininess) * attenuation * intensity;
            final_color += ambient * texdiff + diffuse * texdiff + specular * texspec + ambient * proc_color + diffuse * proc_color + specular * proc_color;
        }
    }
}