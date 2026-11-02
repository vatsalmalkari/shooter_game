#pragma once
#include <glm/glm.hpp>

class Entity {
public:
    struct AABB {
        glm::vec2 center;
        glm::vec2 halfSize;
    };

    virtual ~Entity() = default;
    virtual void Update(float deltaTime) = 0;
    virtual void Draw() = 0;
    virtual AABB GetBounds() const = 0;

    bool IsAlive() const { return aliveFlag_; }
    void Kill() { aliveFlag_ = false; }

protected:
    bool aliveFlag_ = true;
};