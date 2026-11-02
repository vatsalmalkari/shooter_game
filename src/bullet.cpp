#include "bullet.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Bullet::Bullet(glm::vec3 start, glm::vec3 velocity, const Shader& shader)
    : position_(start), velocity_(velocity), shader_(&shader) {
    modelLoc_ = shader.GetUniformLocation("u_ModelMatrix");

    mesh_ = std::make_unique<Mesh>(
        std::vector<float>{
            -0.2f, -0.2f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f,
             0.2f, -0.2f, 0.0f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f,
            -0.2f,  0.2f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
             0.2f,  0.2f, 0.0f,  1.0f, 1.0f, 0.0f,  1.0f, 1.0f
        },
        std::vector<unsigned int>{2, 0, 1, 3, 2, 1}
    );
}

void Bullet::Update(float deltaTime) {
    std::cout << "bullet y: " << position_.y << std::endl;
    std::cout << "bullet x: " << position_.x << std::endl;
    if (!IsAlive()) return;

    position_ += velocity_ * deltaTime;

    if (position_.x > 1.5f || position_.x < -1.5f ||
        position_.y > 1.5f || position_.y < -1.5f) {
        Kill();
    }
}

void Bullet::Draw() {
    shader_->Use();
    glm::mat4 model = glm::translate(glm::mat4(1.0f), position_);
    glUniformMatrix4fv(modelLoc_, 1, GL_FALSE, &model[0][0]);
    mesh_->Draw();
}