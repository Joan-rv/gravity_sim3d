#version 330 core

in vec3 pos;

out vec4 frag_color;

uniform sampler2D skysphere;

#define M_PI 3.1415926535897932384626433832795

void main() {
    vec3 n_pos = normalize(pos);
    float u = 0.5 + atan(n_pos.z, n_pos.x) / (2 * M_PI);
    float v = 0.5 + asin(n_pos.y) / M_PI;
    frag_color = texture(skysphere, vec2(u, v));
}
