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

    // pass render (for temporary debugging)
    void Update() {
        
        // detects if enemies are colliding with the player
        Entity* player = _entityManager->GetFirstEntityWithComponent<PlayerComponent>(); 
        // auto playerHealth = player->GetComponent<HealthComponent>();
        auto playerPos = player->GetComponent<TransformComponent>()->GetPosition();
        auto playerBounds = player->GetComponent<ColliderComponent>()->GetBounds();
        SDL_FRect playerRect = SDL_FRect{playerPos.x, playerPos.y, playerBounds.x, playerBounds.y};
        SDL_FRect playerRectScreen = {};
        _camera->RectWorldToScreen(&playerRect, &playerRectScreen);
        
        for (auto& enemy : _entityManager->GetEntitiesWithComponent<EnemyAIComponent>()) {

            auto enemyTransform = enemy->GetComponent<TransformComponent>();
            assert(enemyTransform != nullptr);
            auto enemyPos = enemyTransform->GetPosition();

            auto enemyCollider = enemy->GetComponent<ColliderComponent>();
            assert(enemyCollider != nullptr);
            auto enemyBounds = enemyCollider->GetBounds();

            SDL_FRect enemyRect = (SDL_FRect){enemyPos.x, enemyPos.y, enemyBounds.x, enemyBounds.y};
            SDL_FRect enemyRectScreen = {};
            _camera->RectWorldToScreen(&enemyRect, &enemyRectScreen);

            if (SDL_HasIntersectionF(&enemyRectScreen, &playerRectScreen)) {
                // playerHealth->TakeDamage();
                // std::cout << "Take damage! " << playerHealth->GetHealth() << " hp" << std::endl;
                std::cout << "Collision between player and " << enemy << std::endl;
                // std::cout << "Collision!" << std::endl;
            }
        }
    }

    // (debug) render collision bounds
    void Render(SDL_Renderer* renderer) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        // render player collision bounds
        Entity* player = _entityManager->GetFirstEntityWithComponent<PlayerComponent>(); 
        auto playerPos = player->GetComponent<TransformComponent>()->GetPosition();
        auto playerBounds = player->GetComponent<ColliderComponent>()->GetBounds();
        SDL_FRect playerRect = SDL_FRect{playerPos.x, playerPos.y, playerBounds.x, playerBounds.y};
        SDL_FRect playerRectScreen = {}; 
        _camera->RectWorldToScreen(&playerRect, &playerRectScreen);
        SDL_SetRenderDrawColor(renderer, 0, 255, 255, 30);
        SDL_RenderFillRectF(renderer, &playerRectScreen);

        // render enemies collision bounds
        for (auto& enemy : _entityManager->GetEntitiesWithComponent<EnemyAIComponent>()) {

            auto enemyTransform = enemy->GetComponent<TransformComponent>();
            assert(enemyTransform != nullptr);
            auto enemyPos = enemyTransform->GetPosition();

            auto enemyCollider = enemy->GetComponent<ColliderComponent>();
            assert(enemyCollider != nullptr);
            auto enemyBounds = enemyCollider->GetBounds();

            SDL_FRect enemyRect = (SDL_FRect){enemyPos.x, enemyPos.y, enemyBounds.x, enemyBounds.y};
            SDL_FRect enemyRectScreen = {};
            _camera->RectWorldToScreen(&enemyRect, &enemyRectScreen);

            SDL_SetRenderDrawColor(renderer, 255, 255, 100, 30);
            SDL_RenderFillRectF(renderer, &enemyRectScreen);
        }
    }
};