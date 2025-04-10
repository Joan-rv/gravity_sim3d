#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <stdint.h>

class Texture {
public:
    Texture();
    ~Texture();
    Texture(const char *path);
    unsigned int id() const;
    void use(uint8_t unit);

private:
    unsigned int id_;
};

#endif
