#include "enemy.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Enemy::Enemy(glm::vec3 start, glm::vec3 velocity, const Shader& shader, const Texture& texture, const Mesh& mesh)
    : position_(start), velocity_(velocity), shader_(&shader), texture_(&texture), mesh_(&mesh) {
    modelLoc_ = shader.GetUniformLocation("u_ModelMatrix");
}

Entity::AABB Enemy::GetBounds() const {
    return { glm::vec2(position_.x, position_.y), glm::vec2(size_ * 0.5f, size_ * 0.5f) };
}

void Enemy::Update(float deltaTime) {
    if (!IsAlive()) return;

    position_ += velocity_ * deltaTime;
    if (position_.y < -1.5f) {
        Kill();
    }
}

void Enemy::Draw() {
    shader_->Use();
    glm::mat4 model = glm::translate(glm::mat4(1.0f), position_);
    model = glm::scale(model, glm::vec3(size_, size_, 1.0f));
    glUniformMatrix4fv(modelLoc_, 1, GL_FALSE, glm::value_ptr(model));
    texture_->Bind(0);
    mesh_->Draw();
}