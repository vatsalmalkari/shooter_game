#pragma once
#include <memory>
#include <glm/glm.hpp>
#include "Entity.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

class Enemy : public Entity {
public:
    Enemy(glm::vec3 start, glm::vec3 velocity, const Shader& shader, const Texture& texture, const Mesh& mesh);

    void Update(float deltaTime) override;
    void Draw() override;
    void OnCollision(Entity* other);
    AABB GetBounds() const override;
    float size_ = 0.35f;
    

private:
    glm::vec3 position_;
    glm::vec3 velocity_;
    const Shader* shader_ = nullptr;
    const Texture* texture_ = nullptr;
    GLint modelLoc_ = -1;
    const Mesh* mesh_ = nullptr;
    const Mesh* quad_ = nullptr;
};