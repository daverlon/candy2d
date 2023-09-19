#pragma once

#include "../stdafx.h"
#include "BaseComponent.h"

class EnemyAIComponent : public Component {
private:
    float _moveSpeed;
    

public:
    EnemyAIComponent() {
            std::cout << "        EnemyAIComponent()" << this << std::endl;

                std::srand(static_cast<unsigned int>(std::time(nullptr)) + reinterpret_cast<std::uintptr_t>(this));


            // Define the range [a, b]
            float a = 50.0f;
            float b = 110.0f;

            // Generate a random float between a and b
            _moveSpeed = a + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (b - a);
            _moveSpeed = 90.0f;

        }

    ~EnemyAIComponent() {
        std::cout << "        ~EnemyAIComponent()" << this << std::endl;
    }

    const float GetMoveSpeed() { return _moveSpeed; } 
};