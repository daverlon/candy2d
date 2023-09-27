#pragma once

#include "../stdafx.h"
#include "../GameClasses/EntityManager.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimatorComponent.h"

class AnimatorSystem {
private:
    EntityManager* _entityManager;

    
public:
    AnimatorSystem(EntityManager* entityManager) :
        _entityManager(entityManager) {
            std::cout << "AnimatorSystem()" <<  this << std::endl;
        }
    ~AnimatorSystem() {
        std::cout << "~AnimatorSystem()" << this << std::endl;
    }

    void Update(const float& dt);
};