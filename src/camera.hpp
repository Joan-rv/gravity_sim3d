#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>

class Camera {
public:
    Camera(glm::vec3 position, float pitch, float yaw,
           glm::vec3 world_up = {0.0f, 1.0f, 0.0f});
    glm::mat4 view() const;
    glm::vec3 position() const;
    float pitch() const;
    float yaw() const;

    void position(glm::vec3 pos);
    void pitch(float pitch);
    void yaw(float yaw);

    void move_left(float magnitude);
    void move_right(float magnitude);
    void move_forward(float magnitude);
    void move_backward(float magnitude);

private:
    void update_vectors();
    glm::vec3 position_;
    float pitch_, yaw_;
    glm::vec3 front_;
    glm::vec3 right_;
    glm::vec3 up_;
    glm::vec3 world_up_;
};

#endif
