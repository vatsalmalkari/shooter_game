#include <vector>
#include <memory>
#include "Entity.hpp"
#include "player.hpp"

class Game {
public:
    void AddEntity(std::unique_ptr<Entity> entity);
    void Update(float deltaTime);
    void Draw();
private:
    std::vector<std::unique_ptr<Entity>> entities_;
};
