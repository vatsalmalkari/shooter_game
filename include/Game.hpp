#pragma once
#include <vector>
#include <memory>
#include "Entity.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include <random>
#include "bullet.hpp"
#include <SDL2/SDL.h>

class Game {
public:
    void AddEntity(std::unique_ptr<Entity> entity);
    void Update(float deltaTime);
    void Draw();
    void SetPlayer(Player* player);
    void SetShader(const Shader& shader); 
    void AddEnemy(std::unique_ptr<Enemy> enemy);
    void AddBullet(std::unique_ptr<Entity> bullet);

    int GetScore() const;
    void AddScore(int points);
    void SetWindow(SDL_Window* window);
    void SubtractScore(int points);
    void SetEnemyTexture(const Texture& enemyTexture);
    bool isGameOver() const;
    void UpdateWindowTitle();
    
private:
    std::vector<std::unique_ptr<Entity>> entities_;
    std::vector<std::unique_ptr<Enemy>> enemies_;
    std::vector<std::unique_ptr<Entity>> bullets_;
    Player* player_ = nullptr;     
    float firecooldown = 0.0f;
    float spawnTimer_ = 1.0f;
    const Shader* shader_ = nullptr;
    const Texture* enemyTexture_ = nullptr;
    std::mt19937 rng_{std::random_device{}()};
    std::uniform_real_distribution<float> spawnXDist_{-1.0f, 1.0f};
    float spawnX_ = 0.0f;
    int score_ = 0;
    SDL_Window* window_ = nullptr;
    bool gameOver_ = false;

};
