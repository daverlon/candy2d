#pragma once

#include "../stdafx.h"
#include "../Utils/GameMath.h"
#include "../Utils/DebugUtils.h"

#include "../GameClasses/EntityManager.h"
#include "../GameClasses/GameCamera.h"
#include "../Systems/ColliderSystem.h"

#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimatorComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/EnemyAIComponent.h"
#include "../Components/SkullBulletComponent.h"

class SkullBulletSystem {
private:
    EntityManager* _entityManager;

public:
    SkullBulletSystem(EntityManager* entityManager) : 

            _entityManager(entityManager)
        {
            std::cout << "EnemyAISystem()" <<  this << std::endl;
        }

    ~SkullBulletSystem() {
        std::cout << "~EnemyAISystem()" << this << std::endl;
    }


    void Update(const float &dt);

    
};