#pragma once
#include <memory>
#include <glm/glm.hpp>
#include "Entity.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

class Bullet : public Entity {
public:
    Bullet(glm::vec3 start, glm::vec3 velocity, const Shader& shader, const Mesh& mesh);

    void Update(float deltaTime) override;
    void Draw() override;
    AABB GetBounds() const override;
    float size_ = 0.15f;

private:
    glm::vec3 position_;
    glm::vec3 velocity_;
    const Shader* shader_ = nullptr;
    GLint modelLoc_ = -1;
    const Mesh* mesh_ = nullptr;
};