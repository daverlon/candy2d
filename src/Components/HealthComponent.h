#pragma once

#include "../stdafx.h"
#include "BaseComponent.h"

class HealthComponent : public Component {
private:
    int _HP;
    int _maxHP;
public:
    HealthComponent(int HP, int maxHP = 100) :
        _HP(HP), _maxHP(maxHP) {
            std::cout << "        HealthComponent()" << this << std::endl;
        }

    ~HealthComponent() {
        std::cout << "        ~HealthComponent()" << this << std::endl;
    }

    inline void TakeDamage(int amount = 1) {
        if (_HP > 0)
            _HP -= amount;
    }

    inline void GainHealth(int amount = 1) {
        if (_HP < _maxHP) {
            _HP += amount;
        }
    }

    inline int GetHealth() { return _HP; }

    
};