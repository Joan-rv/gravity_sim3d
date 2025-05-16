#ifndef ARROW_HPP
#define ARROW_HPP

#include <glm/glm.hpp>
#include <stddef.h>

#include "mesh.hpp"

class Shader;
struct Planet;

class Arrow {
public:
    Arrow(float tip_length, float tip_width, float stem_width,
          size_t resolution);
    void draw(glm::vec3 origin, glm::vec3 end, float scale,
              glm::mat4 projection, glm::mat4 view, Shader &shader) const;
    void draw(const Planet &planet, glm::mat4 projection, glm::mat4 view,
              Shader &shader) const;

private:
    Mesh stem_;
    Mesh tip_;
    float tip_length_;
};

#endif
