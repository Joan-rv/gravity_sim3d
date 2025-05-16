#include <filesystem>
#include <fstream>
#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stdexcept>

#include "shader.hpp"

namespace fs = std::filesystem;

unsigned int Shader::current_used_ = 0;

Shader::Shader() : id_(0) {}

static std::string load_str_from(fs::path path) {
    size_t file_size = std::filesystem::file_size(path);
    std::string buf(file_size, ' ');
    std::ifstream stream(path);
    stream.read(buf.data(), file_size);
    return buf;
}

static std::string load_shader(fs::path path) {
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

class ShaderSource {
public:
    ShaderSource(GLenum type, fs::path path);
    ~ShaderSource() { glDeleteShader(ref_); }
    int ref() { return ref_; }

private:
    int ref_;
};
ShaderSource::ShaderSource(GLenum type, fs::path path)
    : ref_(glCreateShader(type)) {
    std::string src_str = load_shader(path);
    const char *src_ptr = src_str.data();
    const int src_size = src_str.size();
    glShaderSource(ref_, 1, &src_ptr, &src_size);
    glCompileShader(ref_);
    int status;
    glGetShaderiv(ref_, GL_COMPILE_STATUS, &status);
    if (!status) {
        char info_log[512];
        glGetShaderInfoLog(ref_, 512, NULL, info_log);
        std::cerr << "Failed to compile shader\n" << info_log << '\n';
        throw std::runtime_error("Failed to compile shader");
    }
}

Shader::Shader(std::filesystem::path vertex_path,
               std::filesystem::path fragment_path)
    : id_(glCreateProgram()) {

    ShaderSource vertex(GL_VERTEX_SHADER, vertex_path);
    ShaderSource fragment(GL_FRAGMENT_SHADER, fragment_path);

    glAttachShader(id_, vertex.ref());
    glAttachShader(id_, fragment.ref());
    glLinkProgram(id_);
    int status;
    glGetProgramiv(id_, GL_LINK_STATUS, &status);
    if (!status) {
        char info_log[512];
        glGetProgramInfoLog(id_, 512, NULL, info_log);
        std::cerr << "Failed to link shader\n" << info_log << '\n';
        throw std::runtime_error("Failed to link shader");
    }
}

Shader::Shader(std::filesystem::path vertex_path,
               std::filesystem::path geometry_path,
               std::filesystem::path fragment_path)
    : id_(glCreateProgram()) {

    ShaderSource vertex(GL_VERTEX_SHADER, vertex_path);
    ShaderSource fragment(GL_FRAGMENT_SHADER, fragment_path);
    ShaderSource geometry(GL_GEOMETRY_SHADER, geometry_path);

    glAttachShader(id_, vertex.ref());
    glAttachShader(id_, fragment.ref());
    glAttachShader(id_, geometry.ref());
    glLinkProgram(id_);
    int status;
    glGetProgramiv(id_, GL_LINK_STATUS, &status);
    if (!status) {
        char info_log[512];
        glGetProgramInfoLog(id_, 512, NULL, info_log);
        std::cerr << "Failed to link shader\n" << info_log << '\n';
        throw std::runtime_error("Failed to link shader");
    }
}

Shader::~Shader() { glDeleteProgram(id_); }

void Shader::set_mat3(const char *name, glm::mat3 value) {
    assert(current_used_ == id_);
    int location = glGetUniformLocation(id_, name);
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
}
void Shader::set_mat4(const char *name, glm::mat4 value) {
    assert(current_used_ == id_);
    int location = glGetUniformLocation(id_, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}
void Shader::set_vec3(const char *name, glm::vec3 value) {
    assert(current_used_ == id_);
    int location = glGetUniformLocation(id_, name);
    glUniform3fv(location, 1, glm::value_ptr(value));
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

void Shader::set_mvp(glm::mat4 projection, glm::mat4 view, glm::mat4 model) {
    glm::mat4 mv = view * model;
    glm::mat4 mvp = projection * mv;
    glm::mat4 normal_mv = glm::mat3(glm::transpose(glm::inverse(mv)));

    set_mat4("u_mvp", mvp);
    set_mat3("u_normal_mv", normal_mv);
    set_mat4("u_mv", mv);
}

void Shader::use() {
    glUseProgram(id_);
    current_used_ = id_;
}
bool Shader::is_used() const { return current_used_ == id_; }
