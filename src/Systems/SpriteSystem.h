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

class SpriteSystem {
private:
    SDL_Renderer* _renderer;
    EntityManager* _entityManager;
    SDL_Texture* _tileSetTexture;
    GameCamera* _camera;

    
public:
    SpriteSystem(SDL_Renderer* renderer, EntityManager* entityManager, SDL_Texture* tileSetTexture, GameCamera* camera) :
        _renderer(renderer),
        _entityManager(entityManager),
        _tileSetTexture(tileSetTexture),
        _camera(camera) 
        {
            std::cout << "SpriteSystem()" <<  this << std::endl;
        }

    ~SpriteSystem() {
        std::cout << "~SpriteSystem()" << this << std::endl;
    }

    void Render() {
        for (auto &c : _entityManager->GetEntitiesWithComponent<SpriteComponent>()) {
            SpriteComponent* sprite = c->GetComponent<SpriteComponent>();
            if (!sprite->GetActive()) continue;

            TransformComponent* transform = c->GetComponent<TransformComponent>();

            // std::cout << RectToString(sprite->GetSrcRect()) << std::endl;

            glm::vec2 world_pos = transform->GetPosition();

            // draw sprite
            SDL_Rect sprite_rect = sprite->GetSrcRect();
            SDL_FRect sprite_bounds_world = SDL_FRect{world_pos.x, world_pos.y, (float)sprite_rect.w, (float)sprite_rect.h};
            SDL_FRect sprite_bounds_screen = SDL_FRect{};
            _camera->RectWorldToScreen(&sprite_bounds_world, &sprite_bounds_screen);

            SDL_SetRenderDrawColor(_renderer, 255, 0, 255, 255);
            SDL_RenderDrawRectF(_renderer, &sprite_bounds_screen);

            SDL_RenderCopyF(_renderer, _tileSetTexture, &sprite->GetSrcRect(), &sprite_bounds_screen);
        }
    }
};