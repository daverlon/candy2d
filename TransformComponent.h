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

    const glm::vec2& GetPosition() const { return _position; }
    void SetPosition(const glm::vec2& position) { _position = position; }
    void Translate(const glm::vec2& offset) { _position += offset; }
};