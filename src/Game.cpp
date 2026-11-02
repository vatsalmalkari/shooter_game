#include "Game.hpp"
#include <SDL2/SDL.h>
#include <algorithm>
#include <iostream>
float firecooldown = 0.0f;
void Game::SetPlayer(Player* player) {
    player_ = player;
}

void Game::AddEntity(std::unique_ptr<Entity> entity) {
    entities_.push_back(std::move(entity));
}


void Game::Update(float deltaTime) {
    
    bool spaceDown = SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_SPACE];
    if (player_ && spaceDown && !spaceWasDown_ && firecooldown <= 0.0f) {
        AddEntity(player_->fire());
        firecooldown = 0.1f; 
    }
    spaceWasDown_ = spaceDown;
    firecooldown -= deltaTime;
    if (firecooldown < 0.0f) {
        firecooldown = 0.0f;
    }

    for (auto& entity : entities_) {
        entity->Update(deltaTime);
    }
    entities_.erase(
        std::remove_if(entities_.begin(), entities_.end(),
            [](const std::unique_ptr<Entity>& e) { return !e->IsAlive(); }),
        entities_.end());
}

void Game::Draw() {
    for (auto& entity : entities_) {
        entity->Draw();
    }
}