#ifndef SHADER_HPP
#define SHADER_HPP

#include <filesystem>
#include <glm/glm.hpp>

class Shader {
public:
    Shader();
    ~Shader();
    Shader(std::filesystem::path vertex_path,
           std::filesystem::path fragment_path);
    Shader(const Shader &) = delete;
    Shader &operator=(const Shader &) = delete;

    void set_mat4(const char *name, glm::mat4 value);
    void set_vec4(const char *name, glm::vec4 value);
    void set_int(const char *name, int value);

    void use();
    bool is_used() const;

private:
    static unsigned int current_used_;
    const unsigned int id_;
};

#endif
