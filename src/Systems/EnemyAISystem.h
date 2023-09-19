#pragma once

#include "../stdafx.h"
#include "../Utils/GameMath.h"

#include "../GameClasses/EntityManager.h"
#include "../GameClasses/GameCamera.h"
#include "../Systems/ColliderSystem.h"

#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimatorComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/EnemyAIComponent.h"

class EnemyAISystem {
private:
    EntityManager* _entityManager;
    ColliderSystem* _colliderSystem;

public:
    EnemyAISystem(EntityManager* entityManager, ColliderSystem* colliderSystem) : 

            _entityManager(entityManager),
            _colliderSystem(colliderSystem)
        {
            std::cout << "EnemyAISystem()" <<  this << std::endl;
        }

    ~EnemyAISystem() {
        std::cout << "~EnemyAISystem()" << this << std::endl;
    }


    void Update(const float &dt) {

        auto player = _entityManager->GetFirstEntityWithComponent<PlayerComponent>();
        auto playerTransform = player->GetComponent<TransformComponent>();


        for (auto& enemy : _entityManager->GetEntitiesWithComponent<EnemyAIComponent>()) {

            auto ms = enemy->GetComponent<EnemyAIComponent>()->GetMoveSpeed();

            auto transform = enemy->GetComponent<TransformComponent>();

            auto delta = playerTransform->GetPosition() - transform->GetPosition();

            auto movement = glm::normalize(delta) * dt * ms;

            transform->Translate(movement);

        }
    }

    
};