// Entity.hpp
#pragma once
#include <memory>
class Entity {
public:
    virtual ~Entity() = default;
    virtual void Update(float deltaTime) = 0;
    virtual void Draw() = 0;
    bool IsAlive() const { 
        return aliveFlag_; 
    }
    void Kill() { 
        aliveFlag_ = false; 
    }

protected:
    bool aliveFlag_ = true;
};
