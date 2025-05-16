#version 330 core

in VS_OUT {
    vec3 normal;
    vec3 frag_pos;
} fs_in;

out vec4 frag_color;

uniform vec4 u_color;
uniform vec3 u_light_dir;

void main() {
    const float strength_ambient = 0.1;
    const float strength_diffuse = 0.7;
    const float strength_specular = 0.5;

    vec3 normal = normalize(fs_in.normal);

    float diff = max(dot(normal, -u_light_dir), 0.0);

    vec3 reflect_dir = reflect(-u_light_dir, normal);
    vec3 view_dir = normalize(fs_in.frag_pos);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);

    vec3 ambient = strength_ambient * vec3(u_color);
    vec3 diffuse = strength_diffuse * diff * vec3(u_color);
    vec3 specular = strength_specular * spec * vec3(u_color);

    frag_color = vec4(ambient + diffuse + specular, u_color.a);
}
