#include "player.hpp"
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Texture.hpp"
#include "bullet.hpp"

Player::Player(const Shader& shader, const Texture& texture)
    : shader_(&shader), texture_(&texture){
    modelLoc_ = shader.GetUniformLocation("u_ModelMatrix");    
    mesh_ = std::make_unique<Mesh>(
       std::vector<float>{
        -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
         0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f
    },
        std::vector<unsigned int>{2, 0, 1, 3, 2, 1}
    );
}

void Player::Update(float deltaTime) {
    const Uint8* keys = SDL_GetKeyboardState(nullptr);
    if (keys[SDL_SCANCODE_LEFT]) {
        x_ -= speed_ * deltaTime;
    }
    if (keys[SDL_SCANCODE_RIGHT]) {
        x_ += speed_ * deltaTime;
    }
    if (keys[SDL_SCANCODE_UP]) {
        y_ += speed_ * deltaTime;
    }
    if (keys[SDL_SCANCODE_DOWN]) {
        y_ -= speed_ * deltaTime;
    }
}

void Player::Draw() {
    shader_->Use();
    texture_->Bind(0);
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x_, y_, 0.0f));
    glUniformMatrix4fv(modelLoc_, 1, GL_FALSE, &model[0][0]);
    mesh_->Draw();
}

std::unique_ptr<Bullet> Player::fire() {
    return std::make_unique<Bullet>(glm::vec3(x_, y_, 0), glm::vec3(0, 1.0f, 0), *shader_);
}

Entity::AABB Player::GetBounds() const {
    return { glm::vec2(x_, y_), glm::vec2(0.5f, 0.5f) };
}
