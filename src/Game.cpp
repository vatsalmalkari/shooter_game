#include "Game.hpp"

void Game::Update(float deltaTime) {
    for (auto& entity : entities_) {
        entity->Update(deltaTime);
    }
}

void Game::Draw() {
    for (auto& entity : entities_) {
        entity->Draw();
    }
}

void Game::AddEntity(std::unique_ptr<Entity> entity) {
    entities_.push_back(std::move(entity));
}