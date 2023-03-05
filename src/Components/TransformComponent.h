#pragma once

#include "../stdafx.h"
#include "BaseComponent.h"

class TransformComponent : public Component {
private:
    glm::vec2 _position; // world position
public:
    TransformComponent(glm::vec2 position = glm::vec2(0.0f, 0.0f)) :
        _position(position) {
            std::cout << "        TransformComponent()" << this << std::endl;
        }

    ~TransformComponent() {
        std::cout << "        ~TransformComponent()" << this << std::endl;
    }

    inline const glm::vec2& GetPosition() const { return _position; }
    inline void SetPosition(const glm::vec2& position) { _position = position; }
    inline void Translate(const glm::vec2& offset) { _position += offset; }

    inline void MoveSlowly(glm::vec2 targetPos, float speed=5.0f) {
        glm::vec2 target = targetPos;
        glm::vec2 delta = target - GetPosition();
        //std::cout << "delta: " << Vec2toString(delta) << std::endl;
        glm::vec2 move = glm::vec2(delta.x * speed, delta.y * speed);
        Translate(move);
        //std::cout << "delta: " << Vec2toString(cam->position) << std::endl;
    }
};