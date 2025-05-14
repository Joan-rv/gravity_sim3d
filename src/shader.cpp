#include <filesystem>
#include <fstream>
#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "shader.hpp"

unsigned int Shader::current_used_ = 0;

Shader::Shader() : id_(0) {}

static std::string load_str_from(std::filesystem::path path) {
    size_t file_size = std::filesystem::file_size(path);
    std::string buf(file_size, ' ');
    std::ifstream stream(path);
    stream.read(buf.data(), file_size);
    return buf;
}

static std::string load_shader(std::filesystem::path path) {
    std::string data(load_str_from(path));

    size_t line = 0;
    size_t i = 0;
    do {
        size_t next = data.find('\n', i);
        if (next == std::string::npos) {
            next = data.size();
        }

        std::string_view prefix("#include ");
        if (data.compare(i, prefix.size(), prefix) == 0) {
            size_t end = next - i - prefix.size();
            std::filesystem::path include_path =
                path.remove_filename() / (data.substr(i + prefix.size(), end));
            if (std::filesystem::equivalent(include_path, path))
                continue;
            std::string include_str(load_str_from(include_path));
            data.replace(i, next - i, include_str);
            data.replace(i + include_str.size(), 0,
                         "#line " + std::to_string(line) + "\n");
        }

        i = next;
        while (i < data.size() && data[i] == '\n') {
            ++i;
            line++;
        }
    } while (i < data.size() && i != std::string::npos);

    return data;
}

Shader::Shader(std::filesystem::path vertex_path,
               std::filesystem::path fragment_path)
    : id_(glCreateProgram()) {

    std::string vert_str = load_shader(vertex_path);
    const char *vert_ptr = vert_str.data();
    const int vert_size = vert_str.size();
    unsigned int vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &vert_ptr, &vert_size);
    glCompileShader(vert_shader);
    int status;
    glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        char info_log[512];
        glGetShaderInfoLog(vert_shader, 512, NULL, info_log);
        std::cerr << "Failed to compile vertex shader\n" << info_log << '\n';
        throw std::runtime_error("Failed to compile vertex shader");
    }

    std::string frag_str = load_shader(fragment_path);
    const char *frag_ptr = frag_str.data();
    const int frag_size = frag_str.size();
    unsigned int frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &frag_ptr, &frag_size);
    glCompileShader(frag_shader);
    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        char info_log[512];
        glGetShaderInfoLog(frag_shader, 512, NULL, info_log);
        std::cerr << "Failed to compile fragment shader\n" << info_log << '\n';
        throw std::runtime_error("Failed to compile fragment shader");
    }

    glAttachShader(id_, vert_shader);
    glAttachShader(id_, frag_shader);
    glLinkProgram(id_);
    glGetProgramiv(id_, GL_LINK_STATUS, &status);
    if (!status) {
        char info_log[512];
        glGetProgramInfoLog(id_, 512, NULL, info_log);
        std::cerr << "Failed to link shader\n" << info_log << '\n';
        throw std::runtime_error("Failed to link shader");
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
