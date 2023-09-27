#pragma once

#include "../stdafx.h"
#include "../Utils/GameMath.h"

#include "../GameClasses/EntityManager.h"
#include "../GameClasses/GameCamera.h"

#include "../Components/TransformComponent.h"
#include "../Components/ColliderComponent.h"
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


    void Update(const float &dt, const Uint8* keyboardState);

};
