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
    EntityManager *_entityManager;
    GameCamera *_camera; // for zoom

public:
    ColliderSystem(EntityManager *entityManager, GameCamera *camera) : _entityManager(entityManager), _camera(camera) {
        std::cout << "ColliderSystem()" << this << std::endl;
    }
    ~ColliderSystem() {
        std::cout << "~ColliderSystem()" << this << std::endl;
    }

    void Update() {

        // detects if enemies are colliding with the player
        Entity *player = _entityManager->GetFirstEntityWithComponent<PlayerComponent>();
        // auto playerHealth = player->GetComponent<HealthComponent>();
        auto playerBounds = player->GetComponent<ColliderComponent>()->GetBounds();
        auto playerPos = player->GetComponent<TransformComponent>()->GetPosition() - glm::vec2(playerBounds.x, playerBounds.y);
        SDL_FRect playerRect = SDL_FRect{playerPos.x, playerPos.y, playerBounds.z, playerBounds.w};
        SDL_FRect playerRectScreen = {};
        _camera->RectWorldToScreen(&playerRect, &playerRectScreen);

        for (auto &ent1 : _entityManager->GetEntitiesWithComponent<ColliderComponent>()) {
            auto collider1 = ent1->GetComponent<ColliderComponent>();
            assert(collider1 != nullptr);

            auto transform1 = ent1->GetComponent<TransformComponent>();
            assert(transform1 != nullptr);
            auto pos1 = transform1->GetPosition();

            auto bounds1 = collider1->GetBounds();

            SDL_FRect rect1 = {pos1.x - bounds1.x, pos1.y - bounds1.y, bounds1.z, bounds1.w};
            SDL_FRect rect1Screen = {};
            _camera->RectWorldToScreen(&rect1, &rect1Screen);

            for (auto &ent2 : _entityManager->GetEntitiesWithComponent<ColliderComponent>()) {
                if (ent1 == ent2)
                    continue;

                auto collider2 = ent2->GetComponent<ColliderComponent>();
                assert(collider2 != nullptr);

                auto transform2 = ent2->GetComponent<TransformComponent>();
                assert(transform2 != nullptr);
                auto pos2 = transform2->GetPosition();

                auto bounds2 = collider2->GetBounds();

                SDL_FRect rect2 = {pos2.x - bounds2.x, pos2.y - bounds2.y, bounds2.z, bounds2.w};
                SDL_FRect rect2Screen = {};
                _camera->RectWorldToScreen(&rect2, &rect2Screen);

                if (SDL_HasIntersectionF(&rect1Screen, &rect2Screen)) {
                    // intersection between 2 non-solid objects (triggers)

                    // player collision with enemy
                    if (((ent1 == player && ent2->GetComponent<EnemyAIComponent>() != nullptr)) 
                        || (ent2 == player && ent1->GetComponent<EnemyAIComponent>() != nullptr)) {
                        std::cout << "Player damage!" << std::endl;
                    }

                    if (collider1->GetIsTrigger() || collider2->GetIsTrigger()) {
                        std::cout << "Trigger collision between " << ent1 << " and " << ent2 << std::endl;
                    }
                    else if (!collider1->GetIsTrigger() && !collider2->GetIsTrigger())
                    {
                        // Intersection between at least 1 solid object

                        // Calculate the overlap between the rectangles
                        float overlapLeft = rect2Screen.x + rect2Screen.w - rect1Screen.x;
                        float overlapRight = rect1Screen.x + rect1Screen.w - rect2Screen.x;
                        float overlapTop = rect2Screen.y + rect2Screen.h - rect1Screen.y;
                        float overlapBottom = rect1Screen.y + rect1Screen.h - rect2Screen.y;

                        // Find the minimum overlap
                        float minOverlapX = std::min(overlapLeft, overlapRight);
                        float minOverlapY = std::min(overlapTop, overlapBottom);

                        // Separate the entities based on the minimum overlap
                        if (minOverlapX < minOverlapY) {
                            // Separate horizontally
                            float separationX = overlapLeft < overlapRight ? -minOverlapX : minOverlapX;

                            if (!collider1->GetIsUnmoveable() && !collider2->GetIsUnmoveable()) {
                                // If both entities are movable, adjust the positions of both entities
                                pos1.x += separationX * 0.5f;
                                pos2.x -= separationX * 0.5f;
                                transform1->MoveSlowly(pos1, 0.1f);
                                transform2->MoveSlowly(pos2, 0.1f);
                            }
                            else if (!collider1->GetIsUnmoveable()) {
                                // If the first entity is movable, adjust the position of the first entity only
                                pos1.x += separationX;
                                transform1->MoveSlowly(pos1, 0.1f);
                            }
                            else if (!collider2->GetIsUnmoveable()) {
                                // If the second entity is movable, adjust the position of the second entity only
                                pos2.x -= separationX;
                                transform2->MoveSlowly(pos2, 0.1f);
                            }
                        }
                        else {
                            // Separate vertically
                            float separationY = overlapTop < overlapBottom ? -minOverlapY : minOverlapY;

                            if (!collider1->GetIsUnmoveable() && !collider2->GetIsUnmoveable()) {
                                // If both entities are movable, adjust the positions of both entities
                                pos1.y += separationY * 0.5f;
                                pos2.y -= separationY * 0.5f;
                                transform1->MoveSlowly(pos1, 0.1f);
                                transform2->MoveSlowly(pos2, 0.1f);
                            }
                            else if (!collider1->GetIsUnmoveable()) {
                                // If the first entity is movable, adjust the position of the first entity only
                                pos1.y += separationY;
                                transform1->MoveSlowly(pos1, 0.1f);
                            }
                            else if (!collider2->GetIsUnmoveable()) {
                                // If the second entity is movable, adjust the position of the second entity only
                                pos2.y -= separationY;
                                transform2->MoveSlowly(pos2, 0.1f);
                            }
                        }
                    }
                }
            }
        }
    }

    // (debug) render collision bounds
    void Render(SDL_Renderer *renderer) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        // render player collision bounds
        Entity *player = _entityManager->GetFirstEntityWithComponent<PlayerComponent>();
        auto playerBounds = player->GetComponent<ColliderComponent>()->GetBounds();

        auto playerPos = player->GetComponent<TransformComponent>()->GetPosition() - glm::vec2(playerBounds.x, playerBounds.y);
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

            SDL_FRect entRect = (SDL_FRect){entPos.x - entBounds.x, entPos.y - entBounds.y, entBounds.z, entBounds.w};
            SDL_FRect entRectScreen = {};
            _camera->RectWorldToScreen(&entRect, &entRectScreen);

            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 100);
            SDL_RenderFillRectF(renderer, &entRectScreen);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawRectF(renderer, &entRectScreen);
        }
    }
};