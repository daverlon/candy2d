#pragma once

#include "../stdafx.h"
#include "BaseComponent.h"

class ColliderComponent : public Component {
private:
    // glm::vec2 _bounds;
    // glm::vec2 _offset;
    // todo: is_trigger? (no physical blocking)
    glm::vec4 _bounds;
    bool _isTrigger;
    bool _isUnmoveable; // can the object physically interact with others (aka move)

public:

    ColliderComponent(const glm::vec4 &bounds = glm::vec4(0.0f, 0.0f, 16.0f, 16.0f), bool isTrigger = false, bool isUnmoveable = false) :
        _bounds(bounds), _isTrigger(isTrigger), _isUnmoveable(isUnmoveable) {
            std::cout << "        ColliderComponent()" << this << std::endl;
        }

    ~ColliderComponent() {
        std::cout << "        ~ColliderComponent()" << this << std::endl;
    }
    
    // inline glm::vec2 GetOffset() { return _offset; }
    const inline glm::vec4 GetBounds() const { return _bounds; }
    inline bool GetIsTrigger() { return _isTrigger; }
    inline bool GetIsUnmoveable() { return _isUnmoveable; }
};