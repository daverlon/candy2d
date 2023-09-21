#pragma once

#include "../stdafx.h"
#include "../Utils/GameMath.h"

#include "../GameClasses/EntityManager.h"
#include "../GameClasses/GameCamera.h"

#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimatorComponent.h"
#include "../Components/ColliderComponent.h"

#include "../Utils/DebugUtils.h"

class UIRenderSystem {
private:
    SDL_Renderer* _renderer;

public:
    UIRenderSystem(SDL_Renderer* renderer) :
        _renderer(renderer)
        {
            std::cout << "UIRenderSystem() " <<  this << std::endl;
        }

    ~UIRenderSystem() {
        std::cout << "~UIRenderSystem() " << this << std::endl;
    }

    void Render() {
        
        
  
    }
};