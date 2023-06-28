#pragma once

#include "../stdafx.h"
#include "BaseComponent.h"

class EnemyComponent : public Component {
private:

public:
    EnemyComponent(const glm::ivec2 &bounds = glm::ivec2(16, 16)) :
        _bounds(bounds) {
            std::cout << "        EnemyComponent()" << this << std::endl;
        }

    ~EnemyComponent() {
        std::cout << "        ~EnemyComponent()" << this << std::endl;
    }
    
};