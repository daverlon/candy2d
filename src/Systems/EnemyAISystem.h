#pragma once

#include "../stdafx.h"
#include "../Utils/GameMath.h"

#include "../GameClasses/EntityManager.h"
#include "../GameClasses/GameCamera.h"

#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimatorComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/EnemyAIComponent.h"

class EnemyAISystem {
private:
    EntityManager* _entityManager;
    
public:
    EnemyAISystem(EntityManager* entityManager) : 
            _entityManager(entityManager)
        {
            std::cout << "EnemyAISystem()" <<  this << std::endl;
        }

    ~EnemyAISystem() {
        std::cout << "~EnemyAISystem()" << this << std::endl;
    }

    void Update(const float &dt) {

        auto player = _entityManager->GetFirstEntityWithComponent<PlayerComponent>();
        auto playerTransform = player->GetComponent<TransformComponent>();

        const float moveSpeed = 1.4f;

        for (auto& enemy : _entityManager->GetEntitiesWithComponent<EnemyAIComponent>()) {

            auto transform = enemy->GetComponent<TransformComponent>();

            auto delta = playerTransform->GetPosition() - transform->GetPosition();

            transform->Translate(delta * moveSpeed * dt);

            // todo: collider component and collision detection to trigger damage taken
        }
    }
};