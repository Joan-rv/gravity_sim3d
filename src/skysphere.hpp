#ifndef SKYBOX_HPP
#define SKYBOX_HPP

#include <vector>

#include "mesh.hpp"

const float skysphere_vertices[] = {
    // vertices named as if by a top-down view
    // top face
    1.0f, 1.0f, 1.0f,   // lower right
    1.0f, 1.0f, -1.0f,  // upper right
    -1.0f, 1.0f, -1.0f, // upper left
    -1.0f, 1.0f, 1.0f,  // lower left

    // bottom face
    1.0f, -1.0f, 1.0f,   // lower right
    1.0f, -1.0f, -1.0f,  // upper right
    -1.0f, -1.0f, -1.0f, // upper left
    -1.0f, -1.0f, 1.0f,  // lower left

};

const std::vector<VertexAttribute> skysphere_attribs = {
    {0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0}};

const std::vector<unsigned int> skysphere_indices = {
    // faces named as if by a top-down view
    0, 3, 2, 2, 1, 0, // top face
    2, 3, 7, 7, 6, 2, // left face
    4, 0, 1, 1, 5, 4, // right face
    1, 2, 6, 6, 5, 1, // front face
    0, 4, 7, 7, 3, 0, // back face
    6, 7, 4, 4, 5, 6  // bottom face
};

#endif
