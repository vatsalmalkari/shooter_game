#pragma once
#include <vector>
#include <memory>
#include "Entity.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "Shader.hpp"
#include <random>

class Game {
public:
    void AddEntity(std::unique_ptr<Entity> entity);
    void Update(float deltaTime);
    void Draw();
    void SetPlayer(Player* player);
    void SetShader(Shader* shader); 
    void AddEnemy(std::unique_ptr<Enemy> enemy);
    
private:
    std::vector<std::unique_ptr<Entity>> entities_;
    std::vector<std::unique_ptr<Enemy>> enemies_;
    Player* player_ = nullptr;     
    float firecooldown = 0.0f;
    float spawnTimer_ = 1.0f;
    float randomX = 0.0f;
    Shader* shader_ = nullptr;
    std::mt19937 rng_{std::random_device{}()};
    std::uniform_real_distribution<float> spawnXDist_{-1.0f, 1.0f};
    float spawnX_ = 0.0f;
};
