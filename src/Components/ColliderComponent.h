#pragma once

#include "../stdafx.h"
#include "BaseComponent.h"

class ColliderComponent : public Component {
private:
    glm::ivec2 _bounds;
    // todo: is_trigger? (no physical blocking)
public:
    ColliderComponent(const glm::ivec2 &bounds = glm::ivec2(16, 16)) :
        _bounds(bounds) {
            std::cout << "        ColliderComponent()" << this << std::endl;
        }

    ~ColliderComponent() {
        std::cout << "        ~ColliderComponent()" << this << std::endl;
    }
    
    inline glm::vec2 GetBounds() { return _bounds; }
};