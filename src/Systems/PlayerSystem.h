#pragma once

#include "../stdafx.h"
#include "../Utils/GameMath.h"

#include "../GameClasses/EntityManager.h"
#include "../GameClasses/GameCamera.h"

#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimatorComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/SkullBulletComponent.h"

class PlayerSystem {
private:
    EntityManager* _entityManager;
    GameCamera* _camera;
    
public:
    PlayerSystem(EntityManager* entityManager, GameCamera* camera) :
        _entityManager(entityManager),
        _camera(camera) 
        {
            std::cout << "SpriteSystem()" <<  this << std::endl;
        }

    ~PlayerSystem() {
        std::cout << "~SpriteSystem()" << this << std::endl;
    }

    Uint64 bullet_timer = 0; // ticks
    Uint64 bullet_delay = 350; // ticks

    const float ms = 150.0f;

    void Update(const float &dt, const Uint8* keyboardState) {

        auto player = _entityManager->GetFirstEntityWithComponent<PlayerComponent>();
        auto spr = player->GetComponent<SpriteComponent>();
        auto transform = player->GetComponent<TransformComponent>();
        // handle player movement
        {
            glm::vec2 playerMovement = glm::vec2(
                keyboardState[SDL_SCANCODE_D] - 
                keyboardState[SDL_SCANCODE_A],

                keyboardState[SDL_SCANCODE_S] - 
                keyboardState[SDL_SCANCODE_W]
            );

            if (playerMovement.x < 0 && !spr->GetFlipped()) { spr->Flip(); }
            else if (playerMovement.x > 0 && spr->GetFlipped()) { spr->Flip(); }

            if (playerMovement.x != 0.0f && playerMovement.y != 0.0f) {
                playerMovement = glm::normalize(playerMovement);
            }

            playerMovement *= ms * dt;
            //std::cout << Vec2toString(transform->GetPosition());
            transform->Translate(playerMovement);
        }
        _camera->MoveSlowly(transform->GetPosition() + transform->GetOrigin(), 0.2f);


        // handle player bullets
        {
            int mx, my;
            if (SDL_GetMouseState(&mx, &my)&(SDL_BUTTON_LEFT)) {

                if (bullet_timer + bullet_delay <= SDL_GetTicks64()) {
                    bullet_timer = SDL_GetTicks64();

                    std::cout << Vec2toString(glm::ivec2(mx, my)) << std::endl;
                    glm::vec2 direction = -glm::normalize((transform->GetPosition() + transform->GetOrigin()) - _camera->ScreenToWorld(glm::ivec2(mx, my)));

                    auto temp = _entityManager->CreateEntity(
                        new TransformComponent(glm::vec2(transform->GetPosition() + transform->GetOrigin()), glm::vec2(8.0f, 8.0f)),
                        new SpriteComponent(SDL_Rect{288, 432, 16, 16}, SPRITE_LAYER_TOP),
                        new ColliderComponent(glm::vec4(4.0f, 6.0f, 8.0f, 8.0f), true),
                        new SkullBulletComponent(direction, 100.0f)
                    );
                    if (direction.x <= 0) temp->GetComponent<SpriteComponent>()->Flip();
                }
            }
        }
    }
};