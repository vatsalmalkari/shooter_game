#include "Game.hpp"
#include <SDL2/SDL.h>
#include <algorithm>
#include <cmath>
#include <glm/glm.hpp>

namespace {
bool Overlaps(const Entity::AABB& a, const Entity::AABB& b) {
    return std::abs(a.center.x - b.center.x) < a.halfSize.x + b.halfSize.x &&
           std::abs(a.center.y - b.center.y) < a.halfSize.y + b.halfSize.y;
}
}

void Game::SetPlayer(Player* player) {
    player_ = player;
}

void Game::SetShader(const Shader& shader) {
    shader_ = &shader;
}

void Game::AddEntity(std::unique_ptr<Entity> entity) {
    entities_.push_back(std::move(entity));
}

void Game::AddBullet(std::unique_ptr<Entity> bullet) {
    bullets_.push_back(std::move(bullet));
}

void Game::AddEnemy(std::unique_ptr<Enemy> enemy) {
    enemies_.push_back(std::move(enemy));
}

void Game::Update(float deltaTime) {
    firecooldown -= deltaTime;
    if (firecooldown < 0.0f) {
        firecooldown = 0.0f;
    }

    bool spaceDown = SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_SPACE];
    if (player_ && spaceDown && firecooldown <= 0.0f) {
        AddBullet(player_->fire());
        firecooldown = 0.25f;
    }

    spawnTimer_ -= deltaTime;
    if (spawnTimer_ <= 0.0f && shader_) {
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
    for (auto& bullet : bullets_) {
        bullet->Update(deltaTime);
    }
    for (auto& enemy : enemies_) {
        enemy->Update(deltaTime);
    }

    for (auto& bullet : bullets_) {
        if (!bullet->IsAlive()) continue;
        for (auto& enemy : enemies_) {
            if (enemy->IsAlive() && Overlaps(bullet->GetBounds(), enemy->GetBounds())) {
                bullet->Kill();
                enemy->Kill();
                break;
            }
        }
    }

    entities_.erase(std::remove_if(entities_.begin(), entities_.end(),
        [](const std::unique_ptr<Entity>& e) { return !e->IsAlive(); }), entities_.end());
    bullets_.erase(std::remove_if(bullets_.begin(), bullets_.end(),
        [](const std::unique_ptr<Entity>& e) { return !e->IsAlive(); }), bullets_.end());
    enemies_.erase(std::remove_if(enemies_.begin(), enemies_.end(),
        [](const std::unique_ptr<Enemy>& e) { return !e->IsAlive(); }), enemies_.end());
}

void Game::Draw() {
    for (auto& entity : entities_) {
        entity->Draw();
    }
    for (auto& bullet : bullets_) {
        bullet->Draw();
    }
    for (auto& enemy : enemies_) {
        enemy->Draw();
    }
}