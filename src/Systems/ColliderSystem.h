#pragma once

#include "../stdafx.h"
#include "../GameClasses/EntityManager.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimatorComponent.h"
#include "../Components/ColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/EnemyAIComponent.h"
#include "../Components/PlayerComponent.h"
// #include "../Components/HealthComponent.h"

class ColliderSystem {
private:
    EntityManager* _entityManager;
    GameCamera* _camera; // for zoom

    
public:
    ColliderSystem(EntityManager* entityManager, GameCamera* camera) :
        _entityManager(entityManager), _camera(camera) {
            std::cout << "ColliderSystem()" <<  this << std::endl;
        }
    ~ColliderSystem() {
        std::cout << "~ColliderSystem()" << this << std::endl;
    }

    void Update() {
        
        // detects if enemies are colliding with the player
        Entity* player = _entityManager->GetFirstEntityWithComponent<PlayerComponent>(); 
        // auto playerHealth = player->GetComponent<HealthComponent>();

        auto playerPos = player->GetComponent<TransformComponent>()->GetPosition();
        auto playerBounds = player->GetComponent<ColliderComponent>()->GetBounds();
        SDL_FRect playerRect = SDL_FRect{playerPos.x, playerPos.y, playerBounds.x, playerBounds.y};

        //std::cout << RectToString(playerRect) << std::endl;

        for (auto& enemy : _entityManager->GetEntitiesWithComponent<EnemyAIComponent>()) {

            auto enemyTransform = enemy->GetComponent<TransformComponent>();
            if (enemyTransform == nullptr) continue;
            auto enemyPos = enemyTransform->GetPosition();

            auto enemyCollider = enemy->GetComponent<ColliderComponent>();
            if (enemyCollider == nullptr) continue;
            auto enemyBounds = enemyCollider->GetBounds();

            SDL_FRect enemyRect = SDL_FRect{enemyPos.x, enemyPos.y, enemyBounds.x, enemyBounds.y};

            if (SDL_HasIntersectionF(&playerRect, &enemyRect)) {
                // playerHealth->TakeDamage();
                // std::cout << "Take damage! " << playerHealth->GetHealth() << " hp" << std::endl;
                std::cout << "Collision between player and " << enemy << std::endl;
                // std::cout << "Collision!" << std::endl;
            }
        }
    }

    // draw collision bounds (debug)
    void Render(SDL_Renderer* ren) {

        for (auto& enemy : _entityManager->GetEntitiesWithComponent<EnemyAIComponent>()) {
            auto enemyTransform = enemy->GetComponent<TransformComponent>();
            if (enemyTransform == nullptr) continue;
            auto enemyPos = enemyTransform->GetPosition();

            auto enemyCollider = enemy->GetComponent<ColliderComponent>();
            if (enemyCollider == nullptr) continue;
            auto enemyBounds = enemyCollider->GetBounds();

            // world rect
            SDL_FRect enemyRect = SDL_FRect{enemyPos.x, enemyPos.y, enemyBounds.x, enemyBounds.y};

            
            SDL_FRect enemyRectOnScreen = SDL_FRect{};
            _camera->RectWorldToScreen(&enemyRect, &enemyRectOnScreen);




            SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
            SDL_RenderDrawRectF(ren, &enemyRectOnScreen);
        }
    }
};