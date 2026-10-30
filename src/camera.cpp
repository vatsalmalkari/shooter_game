#include "camera.hpp"

Camera::Camera(){
    mEye = glm::vec3(0.0f, 0.0f, 3.0f);
    mViewDirection = glm::vec3(0.0f, 0.0f, -1.0f);
    mUpVector = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(mEye, mViewDirection, mUpVector);
}

void Camera::MoveForward() {
    mEye += mViewDirection;
}
void Camera::MoveBackward() {
    mEye -= mViewDirection;
}
void Camera::MoveLeft() {
    glm::vec3 left = glm::cross(mUpVector, mViewDirection);
    mEye += left;
}
void Camera::MoveRight() {
    glm::vec3 right = glm::cross(mViewDirection, mUpVector);
    mEye += right;
}
void Camera::MoveUp() {
    mEye += mUpVector;
}
void Camera::MoveDown() {
    mEye -= mUpVector;
}

