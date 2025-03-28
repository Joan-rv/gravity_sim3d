#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

#include "shader.hpp"

unsigned int Shader::current_used_ = 0;
Shader::Shader() : id_(0) {}
Shader::Shader(const char *vertex_path, const char *fragment_path)
    : id_(glCreateProgram()) {
    std::ifstream fvert_shader(vertex_path);
    std::vector<char> svert_shader(
        (std::istreambuf_iterator<char>(fvert_shader)),
        std::istreambuf_iterator<char>());
    fvert_shader.close();
    const char *cvert_shader = &svert_shader[0];
    const int cvert_shader_size = svert_shader.size();
    unsigned int vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &cvert_shader, &cvert_shader_size);
    glCompileShader(vert_shader);
    int status;
    glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        char info_log[512];
        glGetShaderInfoLog(vert_shader, 512, NULL, info_log);
        std::cerr << "Failed to compile vertex shader\n" << info_log << '\n';
    }
    std::ifstream ffrag_shader(fragment_path);
    std::vector<char> sfrag_shader(
        (std::istreambuf_iterator<char>(ffrag_shader)),
        std::istreambuf_iterator<char>());
    ffrag_shader.close();
    const char *cfrag_shader = &sfrag_shader[0];
    const int cfrag_shader_size = sfrag_shader.size();
    unsigned int frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &cfrag_shader, &cfrag_shader_size);
    glCompileShader(frag_shader);
    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        char info_log[512];
        glGetShaderInfoLog(frag_shader, 512, NULL, info_log);
        std::cerr << "Failed to compile fragex shader\n" << info_log << '\n';
    }

    glAttachShader(id_, vert_shader);
    glAttachShader(id_, frag_shader);
    glLinkProgram(id_);
    glGetProgramiv(id_, GL_LINK_STATUS, &status);
    if (!status) {
        char info_log[512];
        glGetProgramInfoLog(id_, 512, NULL, info_log);
        std::cerr << "Failed to link shader\n" << info_log << '\n';
    }

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);
}

Shader::~Shader() { glDeleteProgram(id_); }

void Shader::set_mat4(const char *name, glm::mat4 value) {
    assert(current_used_ == id_);
    int location = glGetUniformLocation(id_, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::use() {
    glUseProgram(id_);
    current_used_ = id_;
}
