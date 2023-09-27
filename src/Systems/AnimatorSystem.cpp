#include "AnimatorSystem.h"

void AnimatorSystem::Update(const float& dt) {

    for (auto &e: _entityManager->GetEntitiesWithComponent<AnimatorComponent>()) {

        AnimatorComponent* animator = e->GetComponent<AnimatorComponent>();
        SpriteComponent* sprite = e->GetComponent<SpriteComponent>();

        Animation* anim = animator->GetCurrentAnimation();

        if (anim->GetTimer() >= anim->GetDelay()) {
            anim->IncCurFrame();
            sprite->SetSrcRectX(anim->GetStartFrame().x + anim->GetCurFrame() * anim->GetStartFrame().w);
        } 
        anim->IncTimer(dt);
    }

}