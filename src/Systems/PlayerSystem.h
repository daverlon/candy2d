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

    const float maxSpeed = 150.0f;
    const float ms = 150.0f;

    const float bulletCooldown = 0.05f;
    float bulletCooldownTimer = bulletCooldown;
    const float projectileSpeed = 250.0f;
    const float projectileAngleDelta = 30.0f;
    const int projectileCount = 12; // 360
    int rotation = 0;
    float rotationSpeed = 359.0f;

    const float dashCooldown = 0.4f; // seconds
    float dashCooldownTimer = dashCooldown;
    glm::vec2 dashVec = glm::vec2(0.0f, 0.0f);
    const float dashDuration = 0.1f;
    float dashTimer = 0.0f;
    bool isDashing = false;

    const float dashPower = 700.0f;


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

            //std::cout << Vec2toString(transform->GetPosition());
            float maxMovement = maxSpeed * dt;
            glm::vec2 m = playerMovement * ms * dt;
            if (glm::length(m) > maxMovement) {
              m = glm::normalize(m) * maxMovement;
            }
            transform->Translate(m);

            _camera->MoveSlowly(transform->GetPosition() + transform->GetOrigin(), 0.2f);

            // dash ----- 
            if (dashCooldownTimer >= dashCooldown) {
                if (keyboardState[SDL_SCANCODE_SPACE] && !isDashing) {
                    dashCooldownTimer = 0.0f; // reset dash cooldown timer
                    dashTimer = 0.0f; // reset dash duration timer
                    dashVec = playerMovement * dashPower * dt;
                    isDashing = true;
                }
            }
            else {
                // std::cout << dashCooldownTimer << "/" << dashCooldown << std::endl;
                dashCooldownTimer += dt;
            }

            if (isDashing) {
                if (dashTimer <= dashDuration) {
                    transform->Translate(dashVec);
                    dashTimer += dt;
                }
                else {
                    isDashing = false;
                }
            }
        }


        // handle player bullets
        {
            if (bulletCooldownTimer >= bulletCooldown) {
                int mx, my;
                if (SDL_GetMouseState(&mx, &my) & (SDL_BUTTON_LEFT)) {
                    rotation += rotationSpeed * dt;

                    glm::vec2 direction = -glm::normalize((transform->GetPosition() + transform->GetOrigin()) - _camera->ScreenToWorld(glm::ivec2(mx, my)));
                    // glm::vec2 direction = -glm::normalize(transform->GetPosition() + transform->GetOrigin());

                    for (int i = -projectileCount/2; i < (projectileCount/2) + 1; i++) {
                        float angle = i * projectileAngleDelta + (rotation % 361);
                        // std::cout << angle << std::endl;

                        glm::vec2 dir(
                            cos(glm::radians(angle)) * direction.x - sin(glm::radians(angle)) * direction.y, 
                            sin(glm::radians(angle)) * direction.x + cos(glm::radians(angle)) * direction.y
                        );

                        auto temp = _entityManager->CreateEntity(

                            new TransformComponent(glm::vec2(transform->GetPosition() + transform->GetOrigin() - glm::vec2(8.0f, 12.0f)), glm::vec2(8.0f, 8.0f)),
                            new SpriteComponent(SDL_Rect{288, 432, 16, 16}, SPRITE_LAYER_TOP),
                            new ColliderComponent(glm::vec4(4.0f, 6.0f, 8.0f, 8.0f), true),
                            new SkullBulletComponent(dir * 2.0f, projectileSpeed));

                        if (dir.x >= 0.0f) temp->GetComponent<SpriteComponent>()->Flip();
                    }

                    bulletCooldownTimer = 0.0f;
                }
            } 
            else {
                bulletCooldownTimer += dt;
            }
        }
    }
};
