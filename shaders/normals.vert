#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;

out VS_OUT {
    vec3 normal;
} vs_out;

uniform mat4 view;
uniform mat4 model;

void main() {
    gl_Position = view * model * vec4(a_pos, 1.0);
    vs_out.normal = normalize(mat3(transpose(inverse(view * model))) * a_normal);
}
