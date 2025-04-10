#version 330 core
layout (location = 0) in vec3 a_pos;

out vec3 pos;

uniform mat4 projection;
uniform mat4 view;

void main() {
    pos = a_pos;
    gl_Position = (projection * view * vec4(a_pos, 1.0)).xyww;
    //gl_Position = projection * view * vec4(a_pos.xy, 1.0, 1.0);
}
