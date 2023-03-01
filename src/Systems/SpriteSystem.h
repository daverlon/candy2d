#pragma once

#include "../stdafx.h"
#include "../Utils/GameMath.h"

#include "../GameClasses/EntityManager.h"
#include "../GameClasses/GameCamera.h"

#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimatorComponent.h"

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

            TransformComponent* transform = c->GetComponent<TransformComponent>();
            SpriteComponent* sprite = c->GetComponent<SpriteComponent>();
            // std::cout << RectToString(sprite->GetSrcRect()) << std::endl;

            glm::vec2 world_pos = transform->GetPosition();
            glm::vec2 on_screen_position = _camera->WorldToScreen(glm::vec2(world_pos.x, world_pos.y));

            glm::vec2 on_screen_size = glm::vec2(0.0f, 0.0f);
            on_screen_size += glm::vec2(sprite->GetSrcRect().w, sprite->GetSrcRect().h);
            on_screen_size *= _camera->GetZoom();

            SDL_FRect on_screen_rect = Vec2Vec2toRect(on_screen_position, on_screen_size);

            SDL_RenderCopyF(_renderer, _tileSetTexture, &sprite->GetSrcRect(), &on_screen_rect);
        }
    }
};