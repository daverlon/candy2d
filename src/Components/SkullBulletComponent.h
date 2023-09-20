#pragma once

#include "../stdafx.h"
#include "BaseComponent.h"

// could be called projectile component?: consider generic projectile component
class SkullBulletComponent : public Component {
private:
    glm::vec2 _direction;
    float _speed;
    
public:
    int TTL; // time to live

    SkullBulletComponent(glm::vec2 direction, float speed) : _direction(direction), _speed(speed), TTL(100) {
         std::cout << "        SkullBulletComponent()" << this << std::endl;
        }

    ~SkullBulletComponent() {
        std::cout << "        ~SkullBulletComponent()" << this << std::endl;
    }

    const float GetSpeed() { return _speed; }
    const glm::vec2 GetDirection() { return _direction; }
};