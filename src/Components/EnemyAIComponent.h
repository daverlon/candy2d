#pragma once

#include "../stdafx.h"
#include "BaseComponent.h"

class EnemyAIComponent : public Component {
private:
    // float _moveSpeed;

public:
    EnemyAIComponent() {
            std::cout << "        EnemyAIComponent()" << this << std::endl;
        }

    ~EnemyAIComponent() {
        std::cout << "        ~EnemyAIComponent()" << this << std::endl;
    }
};