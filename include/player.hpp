#pragma once
#include "Entity.hpp"
#include <memory>
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "bullet.hpp"


class Player  : public Entity {
public:
    Player(const Shader& shader, const Texture& texture, const Mesh& mesh);
    void Update(float deltaTime) override;
    void Draw() override;
    std::unique_ptr<Bullet> fire();
    Entity::AABB GetBounds() const override;
    float size_ = 0.75f;
    
    
private:
    const Mesh* mesh_ = nullptr;
    float x_ = 0.0f;
    float y_ = 0.0f;
    float speed_ = 1.0f;
    const Shader* shader_ = nullptr;
    const Texture* texture_ = nullptr;
    GLint modelLoc_ = -1;
    GLint shaderProgramLoc_ = -1;
};