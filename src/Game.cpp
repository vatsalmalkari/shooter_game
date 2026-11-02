#include "Game.hpp"
#include <SDL2/SDL.h>
#include <algorithm>
#include <iostream>

void Game::SetPlayer(Player* player) {
    player_ = player;
}

void Game::AddEntity(std::unique_ptr<Entity> entity) {
    entities_.push_back(std::move(entity));
    std::cout << "bullet fired, entities: " << entities_.size() << std::endl;
}


void Game::Update(float deltaTime) {
    
    bool spaceDown = SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_SPACE];
    if (player_ && spaceDown && !spaceWasDown_) {
        AddEntity(player_->fire());
    }
    spaceWasDown_ = spaceDown;

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