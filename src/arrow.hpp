#ifndef ARROW_HPP
#define ARROW_HPP

#include <stddef.h>

#include "mesh.hpp"

class Shader;

class Arrow {
public:
    Arrow(float tip_width, float stem_width, size_t resolution);
    void draw(Shader &shader) const;

private:
    Mesh stem_;
    Mesh tip_;
};

#endif
