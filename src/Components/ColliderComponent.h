#pragma once

#include "../stdafx.h"
#include "BaseComponent.h"

class ColliderComponent : public Component {
private:
    // glm::vec2 _bounds;
    // glm::vec2 _offset;
    // todo: is_trigger? (no physical blocking)
    glm::vec4 _bounds;

public:

    ColliderComponent(const glm::vec4 &bounds = glm::vec4(0.0f, 0.0f, 16.0f, 16.0f)) :
        _bounds(bounds) {
            std::cout << "        ColliderComponent()" << this << std::endl;
        }

    // ColliderComponent(const glm::vec2 &bounds = glm::vec2(16.0f, 16.0f), const glm::vec2 &offset = glm::vec2(0.0f, 0.0f)) : 
    //     _bounds(bounds), _offset(offset) {
    //         std::cout << "        ColliderComponent()" << this << std::endl;
    //     }

    ~ColliderComponent() {
        std::cout << "        ~ColliderComponent()" << this << std::endl;
    }
    
    // inline glm::vec2 GetOffset() { return _offset; }
    inline glm::vec4 GetBounds() { return _bounds; }
};