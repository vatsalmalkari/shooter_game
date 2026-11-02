#include <vector>
#include <memory>
#include "Entity.hpp"
#include "player.hpp"

class Game {
public:
    void AddEntity(std::unique_ptr<Entity> entity);
    void Update(float deltaTime);
    void Draw();
    void SetPlayer(Player* player);
private:
    std::vector<std::unique_ptr<Entity>> entities_;
    Player* player_ = nullptr;     
    bool spaceWasDown_ = false; 
};
