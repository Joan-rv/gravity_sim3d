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
    Shader(std::filesystem::path vertex_path,
           std::filesystem::path geometry_path,
           std::filesystem::path fragment_path);
    Shader(const Shader &) = delete;
    Shader &operator=(const Shader &) = delete;

    void set_mat3(const char *name, glm::mat3 value);
    void set_mat4(const char *name, glm::mat4 value);
    void set_vec3(const char *name, glm::vec3 value);
    void set_vec4(const char *name, glm::vec4 value);
    void set_int(const char *name, int value);

    void set_mvp(glm::mat4 projection, glm::mat4 view, glm::mat4 model);

    void use();
    bool is_used() const;

private:
    static unsigned int current_used_;
    const unsigned int id_;
};

#endif
