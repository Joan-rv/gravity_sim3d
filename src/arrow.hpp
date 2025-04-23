#ifndef ARROW_HPP
#define ARROW_HPP

#include <glm/glm.hpp>
#include <stddef.h>

#include "mesh.hpp"

class Shader;

class Arrow {
public:
    Arrow(float tip_length, float tip_width, float stem_width,
          size_t resolution);
    void draw(glm::vec3 origin, glm::vec3 end, Shader &shader) const;

private:
    Mesh stem_;
    Mesh tip_;
    float tip_length_;
};

#endif
