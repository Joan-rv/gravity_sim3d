#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;


out VS_OUT {
    vec3 normal;
    vec3 frag_pos;
} vs_out;

uniform mat4 u_mvp;
uniform mat3 u_normal_mv;
uniform mat4 u_mv;

void main() {
    gl_Position = u_mvp * vec4(a_pos, 1.0);
    vs_out.normal = u_normal_mv * a_normal;
    vs_out.frag_pos = vec3(u_mv * vec4(a_pos, 1.0));
}
