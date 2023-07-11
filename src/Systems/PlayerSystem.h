#pragma once

#include "../stdafx.h"
#include "../Utils/GameMath.h"

#include "../GameClasses/EntityManager.h"
#include "../GameClasses/GameCamera.h"

#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimatorComponent.h"
#include "../Components/PlayerComponent.h"

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

    void Update(const float &dt, const Uint8* keyboardState) {

        // handle player movement
        const float ms = 120.0f;

        for (auto& player : _entityManager->GetEntitiesWithComponent<PlayerComponent>()) {

            auto spr = player->GetComponent<SpriteComponent>();
            assert(spr != nullptr);
            // spr->SetAngle(spr->GetAngle()+1); //spin!

            spr->SetAngle(spr->GetAngle() + 1);

            auto transform = player->GetComponent<TransformComponent>();
            assert(transform != nullptr);

            glm::vec2 playerMovement = glm::vec2(
                keyboardState[SDL_SCANCODE_A] - 
                keyboardState[SDL_SCANCODE_D],

                keyboardState[SDL_SCANCODE_W] - 
                keyboardState[SDL_SCANCODE_S]
            );

            if (playerMovement.x < 0 && spr->GetFlipped()) { spr->Flip(); }
            else if (playerMovement.x > 0 && !spr->GetFlipped()) { spr->Flip(); }

            if (playerMovement.x != 0.0f && playerMovement.y != 0.0f) {
                playerMovement = glm::normalize(playerMovement);
            }

            playerMovement *= ms * dt;

            // transform->Translate(playerMovement);
            transform->MoveSlowly(transform->GetPosition() + playerMovement, 1.0f);

            _camera->MoveSlowly(transform->GetPosition() - glm::vec2(8.0f, 8.0f), 0.3f);
        }
    }
};