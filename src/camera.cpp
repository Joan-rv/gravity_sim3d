#include <algorithm>
#include <glm/ext/matrix_transform.hpp>

#include "camera.hpp"
#include "pi.hpp"

Camera::Camera(glm::vec3 position, float pitch, float yaw, glm::vec3 world_up)
    : position_(position), pitch_(pitch), yaw_(yaw), world_up_(world_up) {
    update_vectors_();
}

glm::mat4 Camera::view() const {
    return glm::lookAt(position_, position_ + front_, up_);
}
glm::vec3 Camera::position() const { return position_; }
float Camera::pitch() const { return pitch_; }
float Camera::yaw() const { return yaw_; }

void Camera::position(glm::vec3 pos) {
    position_ = pos;
    update_vectors_();
}
void Camera::pitch(float pitch) {
    pitch_ = std::clamp(pitch, -PI_2 + 0.01f, PI_2 - 0.01f);
    update_vectors_();
}
void Camera::yaw(float yaw) {
    yaw_ = yaw;
    update_vectors_();
}

void Camera::move_left(float magnitude) { position_ -= right_ * magnitude; }
void Camera::move_right(float magnitude) { position_ += right_ * magnitude; }
void Camera::move_forward(float magnitude) { position_ += front_ * magnitude; }
void Camera::move_backward(float magnitude) { position_ -= front_ * magnitude; }

void Camera::update_vectors_() {
    front_.x = cos(yaw_) * cos(pitch_);
    front_.y = sin(pitch_);
    front_.z = sin(yaw_) * cos(pitch_);

    front_ = glm::normalize(front_);
    right_ = glm::normalize(glm::cross(front_, world_up_));
    up_ = glm::normalize(glm::cross(right_, front_));
}
