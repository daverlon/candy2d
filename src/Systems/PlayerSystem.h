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
        const float ms = 150.0f;

        for (auto& player : _entityManager->GetEntitiesWithComponent<PlayerComponent>()) {

            auto transform = player->GetComponent<TransformComponent>();
            assert(transform != nullptr);

            glm::vec2 playerMovement = glm::vec2(
                (keyboardState[SDL_SCANCODE_D] * (-ms*dt)) - 
                (keyboardState[SDL_SCANCODE_A] * (-ms*dt)),

                (keyboardState[SDL_SCANCODE_S] * (-ms*dt)) -
                (keyboardState[SDL_SCANCODE_W] * (-ms*dt))
            );

            transform->Translate(playerMovement);

            _camera->MoveSlowly(transform->GetPosition() - glm::vec2(16.0f, 16.0f), 0.08f);
        }
    }
};