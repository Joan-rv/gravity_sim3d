#include <glm/ext/matrix_transform.hpp>

#include "camera.hpp"

Camera::Camera(glm::vec3 position, float pitch, float yaw, glm::vec3 world_up)
    : position_(position), pitch_(pitch), yaw_(yaw), world_up_(world_up) {
    update_vectors();
}

glm::mat4 Camera::view() const {
    return glm::lookAt(position_, position_ + front_, up_);
}
glm::vec3 Camera::position() const { return position_; }
float Camera::pitch() const { return pitch_; }
float Camera::yaw() const { return yaw_; }

void Camera::position(glm::vec3 pos) {
    position_ = pos;
    update_vectors();
}
void Camera::pitch(float pitch) {
    pitch_ = pitch;
    update_vectors();
}
void Camera::yaw(float yaw) {
    yaw_ = yaw;
    update_vectors();
}

void Camera::update_vectors() {
    front_.x = cos(yaw_) * cos(pitch_);
    front_.y = sin(pitch_);
    front_.z = sin(yaw_) * cos(pitch_);

    front_ = glm::normalize(front_);
    right_ = glm::normalize(glm::cross(front_, world_up_));
    up_ = glm::normalize(glm::cross(right_, front_));
}
