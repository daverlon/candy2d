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

public:
    EnemyAISystem(EntityManager* entityManager) : 

            _entityManager(entityManager)
        {
            std::cout << "EnemyAISystem()" <<  this << std::endl;
        }

    ~EnemyAISystem() {
        std::cout << "~EnemyAISystem()" << this << std::endl;
    }


    void Update(const float &dt);
};