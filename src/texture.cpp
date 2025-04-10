#include <cassert>
#include <glad/glad.h>
#include <iostream>
#include <stb_image.h>

#include "texture.hpp"

Texture::Texture() : id_(0) {}
Texture::~Texture() { glDeleteTextures(1, &id_); }

Texture::Texture(const char *path) {
    stbi_set_flip_vertically_on_load(true);
    glGenTextures(1, &id_);
    glBindTexture(GL_TEXTURE_2D, id_);
    int width, height, nr_channels;
    unsigned char *data = stbi_load(path, &width, &height, &nr_channels, 3);
    assert(nr_channels == 3);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture at: " << path << '\n';
    }
    stbi_image_free(data);
}

unsigned int Texture::id() const { return id_; }

void Texture::use(uint8_t unit) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, id_);
}
