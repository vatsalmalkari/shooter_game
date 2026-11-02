#include "Game.hpp"
#include <SDL2/SDL.h>
#include <algorithm>
#include <random>
#include <glm/glm.hpp>

void Game::SetPlayer(Player* player) {
    player_ = player;
}

void Game::AddEntity(std::unique_ptr<Entity> entity) {
    entities_.push_back(std::move(entity));
}

void Game::SetShader(Shader* shader) {
    shader_ = shader;
}

void Game::AddEnemy(std::unique_ptr<Enemy> enemy) {
    enemies_.push_back(std::move(enemy));
}


void Game::Update(float deltaTime) {
    firecooldown -= deltaTime;
    if (firecooldown < 0.0f){
        firecooldown = 0.0f;
    }
    bool spaceDown = SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_SPACE];
    if (player_ && spaceDown && firecooldown <= 0.0f) {
        AddEntity(player_->fire());
        firecooldown = 0.25f;
    }
    spawnTimer_ -= deltaTime;
    if (spawnTimer_ <= 0.0f) {
        spawnX_ = spawnXDist_(rng_);
        AddEnemy(std::make_unique<Enemy>(
            glm::vec3(spawnX_, 1.5f, 0.0f),
            glm::vec3(0.0f, -0.5f, 0.0f),
            *shader_));
        spawnTimer_ = 1.0f;
    }
    for (auto& entity : entities_) {
        entity->Update(deltaTime);
    }
    for (auto& enemy : enemies_) {
        enemy->Update(deltaTime);
    }
    enemies_.erase( std::remove_if(enemies_.begin(), enemies_.end(), [](const std::unique_ptr<Enemy>& e) { return !e->IsAlive(); }), enemies_.end());
    
    entities_.erase(std::remove_if(entities_.begin(), entities_.end(), [](const std::unique_ptr<Entity>& e) { return !e->IsAlive(); }), entities_.end());
}

void Game::Draw() {
    for (auto& entity : entities_) {
        entity->Draw();
    }
    for (auto& enemy : enemies_) {
        enemy->Draw();
    }
}