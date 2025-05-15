#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;

out VS_OUT {
    vec3 normal;
    vec3 frag_pos;
    vec3 light_dir;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
    gl_Position = projection * view * model * vec4(a_pos, 1.0);
    vs_out.normal = normalize(mat3(transpose(inverse(model))) * a_normal);
    vs_out.frag_pos = vec3(view * model * vec4(a_pos, 1.0));
    vs_out.light_dir = -normalize(mat3(view) * vec3(0.2, -1.0, -0.1));
}
