#ifndef SHADER_HPP
#define SHADER_HPP

#include <glm/glm.hpp>

class Shader {
public:
    Shader();
    ~Shader();
    Shader(const char *vertex_path, const char *fragment_path);
    Shader(const Shader &) = delete;
    Shader &operator=(const Shader &) = delete;

    void set_mat4(const char *name, glm::mat4 value);

    void use();

private:
    static unsigned int current_used_;
    const unsigned int id_;
};

#endif
