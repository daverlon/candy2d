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
            std::cout << "AnimatorSystem()" << std::endl;
        }
    ~AnimatorSystem() {
        std::cout << "~AnimatorSystem()" << std::endl;
    }

    void Update(const float& dt) {
        
        for (auto &e: _entityManager->GetEntitiesWithComponent<AnimatorComponent>()) {

            AnimatorComponent* animator = e->GetComponent<AnimatorComponent>();
            SpriteComponent* sprite = e->GetComponent<SpriteComponent>();

            Animation* anim = &animator->GetCurrentAnimation();

            if (anim->GetTimer() >= anim->GetDelay()) {
                anim->IncCurFrame();
                sprite->SetSrcRectX(anim->GetStartFrame().x + anim->GetCurFrame() * anim->GetStartFrame().w);
            } 
            anim->IncTimer(dt);
        }
    }
};