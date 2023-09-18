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

#include "../Utils/GameMath.h"

class ColliderSystem {
private:
    EntityManager *_entityManager;
    GameCamera *_camera; // for zoom

    // const int grid_width = 10;
    // const int grid_height = 10;
    // const float cell_size = 10.0f; // should be the same as the grid width/height (since they are perfect squares) except floating point

public:
    ColliderSystem(EntityManager *entityManager, GameCamera *camera) : 
    _entityManager(entityManager), 
    _camera(camera) {
        std::cout << "ColliderSystem()" << this << std::endl;

    }

    ~ColliderSystem() {
        std::cout << "~ColliderSystem()" << this << std::endl;
    }

    void Update(const float &dt) {
        // Perform collision checks only between potentially colliding entities

        auto es = _entityManager->GetEntitiesWithComponent<ColliderComponent>();
        int ei = es.size();
        for (int i = 0; i < ei; ++i) {
            for (int j = i + 1; j < ei; ++j) {
                auto entity1 = es[i];
                auto entity2 = es[j];

                if (entity1 == entity2) continue;

                auto collider1 = entity1->GetComponent<ColliderComponent>();
                auto collider2 = entity2->GetComponent<ColliderComponent>();

                // continue;
                if (collider1->GetIsTrigger() || collider2->GetIsTrigger()) continue;

                auto transform1 = entity1->GetComponent<TransformComponent>();
                auto transform2 = entity2->GetComponent<TransformComponent>();

                auto pos1 = transform1->GetPosition();
                auto pos2 = transform2->GetPosition();

                auto bounds1 = collider1->GetBounds();
                auto bounds2 = collider2->GetBounds();

                auto rect1 = SDL_FRect{pos1.x + bounds1.x, pos1.y + bounds1.y, bounds1.z, bounds1.w};
                auto rect2 = SDL_FRect{pos2.x + bounds2.x, pos2.y + bounds2.y, bounds2.z, bounds2.w};

                if (SDL_HasIntersectionF(&rect1, &rect2)) {
                    std::cout << entity1 << RectToString(rect1) << 
                    " <-> " << entity2 << RectToString(rect2) << std::endl;
      
                }
            }
        }
    }

    // (debug) render collision bounds
    void Render(SDL_Renderer *renderer) {
        // return;
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        // render player collision bounds
        Entity *player = _entityManager->GetFirstEntityWithComponent<PlayerComponent>();
        auto playerBounds = player->GetComponent<ColliderComponent>()->GetBounds();

        auto playerPos = player->GetComponent<TransformComponent>()->GetPosition() + glm::vec2(playerBounds.x, playerBounds.y);
        SDL_FRect playerRect = SDL_FRect{playerPos.x, playerPos.y, playerBounds.z, playerBounds.w};
        SDL_FRect playerRectScreen = {};
        _camera->RectWorldToScreen(&playerRect, &playerRectScreen);
        SDL_SetRenderDrawColor(renderer, 0, 255, 255, 30);
        SDL_RenderFillRectF(renderer, &playerRectScreen);

        // render collision entities (objects)
        for (auto &ent : _entityManager->GetEntitiesWithComponent<ColliderComponent>()) {

            auto entCollider = ent->GetComponent<ColliderComponent>();
            assert(entCollider != nullptr);

            auto entTransform = ent->GetComponent<TransformComponent>();
            assert(entTransform != nullptr);
            auto entPos = entTransform->GetPosition();

            auto entBounds = entCollider->GetBounds();

            SDL_FRect entRect = (SDL_FRect){entPos.x + entBounds.x, entPos.y + entBounds.y, entBounds.z, entBounds.w};
            SDL_FRect entRectScreen = {};
            _camera->RectWorldToScreen(&entRect, &entRectScreen);

            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 100);
            SDL_RenderFillRectF(renderer, &entRectScreen);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawRectF(renderer, &entRectScreen);
        }
    }
};