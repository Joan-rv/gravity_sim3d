#include <filesystem>
#include <fstream>
#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

#include "shader.hpp"

unsigned int Shader::current_used_ = 0;

Shader::Shader() : id_(0) {}

Shader::Shader(const char *vertex_path, const char *fragment_path)
    : id_(glCreateProgram()) {
    int vert_size = std::filesystem::file_size(vertex_path);
    std::vector<char> vert_buf(vert_size);
    std::ifstream vert_stream(vertex_path, std::ios::binary);
    vert_stream.read(&vert_buf[0], vert_size);

    const char *vert_str = &vert_buf[0];
    unsigned int vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &vert_str, &vert_size);
    glCompileShader(vert_shader);
    int status;
    glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        char info_log[512];
        glGetShaderInfoLog(vert_shader, 512, NULL, info_log);
        std::cerr << "Failed to compile vertex shader\n" << info_log << '\n';
    }

    int frag_size = std::filesystem::file_size(fragment_path);
    std::vector<char> frag_buf(frag_size);
    std::ifstream frag_stream(fragment_path);
    frag_stream.read(&frag_buf[0], frag_size);

    const char *frag_str = &frag_buf[0];
    unsigned int frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &frag_str, &frag_size);
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
void Shader::set_vec4(const char *name, glm::vec4 value) {
    assert(current_used_ == id_);
    int location = glGetUniformLocation(id_, name);
    glUniform4fv(location, 1, glm::value_ptr(value));
}
void Shader::set_int(const char *name, int value) {
    assert(current_used_ == id_);
    int location = glGetUniformLocation(id_, name);
    glUniform1i(location, value);
}

void Shader::use() {
    glUseProgram(id_);
    current_used_ = id_;
}
bool Shader::is_used() const { return current_used_ == id_; }
