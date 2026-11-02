#pragma once
#include <memory>
#include <glm/glm.hpp>
#include "Entity.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"

class Enemy : public Entity {
public:
    Enemy(glm::vec3 start, glm::vec3 velocity, const Shader& shader);

    void Update(float deltaTime) override;
    void Draw() override;
    void OnCollision(Entity* other);
    AABB GetBounds() const override;
    

private:
    glm::vec3 position_;
    glm::vec3 velocity_;
    const Shader* shader_ = nullptr;
    GLint modelLoc_ = -1;
    std::unique_ptr<Mesh> mesh_;
};