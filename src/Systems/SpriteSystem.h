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

   std::map<int, std::vector<Entity*>> _entitiesByLayer;  // Map to store entities grouped by layer
    bool _needsLayerUpdate;  // Flag to indicate if the layer grouping needs update



    
public:
    SpriteSystem(SDL_Renderer* renderer, EntityManager* entityManager, SDL_Texture* tileSetTexture, GameCamera* camera) :
        _renderer(renderer),
        _entityManager(entityManager),
        _tileSetTexture(tileSetTexture),
        _camera(camera),
        _needsLayerUpdate(true)
        {
            std::cout << "SpriteSystem() " <<  this << std::endl;
        }

    ~SpriteSystem() {
        std::cout << "~SpriteSystem() " << this << std::endl;
    }

    void UpdateEntitiesByLayer() {
        _entitiesByLayer.clear();
        for (auto& entity : _entityManager->GetEntitiesWithComponent<SpriteComponent>()) {
            SpriteComponent* sprite = entity->GetComponent<SpriteComponent>();
            assert(sprite != nullptr);
            if (!sprite->GetActive()) continue;

            _entitiesByLayer[sprite->GetLayer()].push_back(entity);
        }
        _needsLayerUpdate = false;
    }

    void YSort() {
         // Sort the entities within the YSORT layer based on their Y positions
        for (auto& layerEntry : _entitiesByLayer) {
            int layer = layerEntry.first;
            std::vector<Entity*>& entities = layerEntry.second;

            if (layer == SPRITE_LAYER_YSORT) {
                std::sort(entities.begin(), entities.end(), [](Entity* entity1, Entity* entity2) {
                    TransformComponent* transform1 = entity1->GetComponent<TransformComponent>();
                    assert(transform1 != nullptr);
                    TransformComponent* transform2 = entity2->GetComponent<TransformComponent>();
                    assert(transform2 != nullptr);

                    // may not need this if check, since the transforms are being asserted
                    return (transform1->GetPosition().y + transform1->GetOrigin().y) 
                        < (transform2->GetPosition().y + transform2->GetOrigin().y);

                    return false;
                });
            }
        }
    }

    void Render() {

        if (_needsLayerUpdate) {
            UpdateEntitiesByLayer();
        }

        YSort();

        // Render sprites by layer
        for (const auto& layerEntry : _entitiesByLayer) {
            const std::vector<Entity*>& entities = layerEntry.second;

            for (Entity* entity : entities) {
                SpriteComponent* sprite = entity->GetComponent<SpriteComponent>();
                TransformComponent* transform = entity->GetComponent<TransformComponent>();
                assert(sprite != nullptr && transform != nullptr);

                glm::vec2 world_pos = transform->GetPosition();

                SDL_Rect sprite_rect = sprite->GetSrcRect();
                SDL_FRect sprite_bounds_world = SDL_FRect{world_pos.x, world_pos.y, (float)sprite_rect.w, (float)sprite_rect.h};
                SDL_FRect sprite_bounds_screen = SDL_FRect{};
                _camera->RectWorldToScreen(&sprite_bounds_world, &sprite_bounds_screen);

                if (sprite->GetFlipped())
                    SDL_RenderCopyExF(_renderer, _tileSetTexture, &sprite->GetSrcRect(), &sprite_bounds_screen, sprite->GetAngle(), NULL, SDL_FLIP_HORIZONTAL);
                else
                    SDL_RenderCopyExF(_renderer, _tileSetTexture, &sprite->GetSrcRect(), &sprite_bounds_screen, sprite->GetAngle(), NULL, SDL_FLIP_NONE);

#ifdef DEBUG
                // render bounds (debug)
                SDL_SetRenderDrawColor(_renderer, 255, 0, 255, 255);
                SDL_RenderDrawRectF(_renderer, &sprite_bounds_screen);



                // render origin (debug)
                SDL_SetRenderDrawColor(_renderer, 0, 255, 0, 255);
                glm::vec2 origScreen = _camera->WorldToScreen(world_pos + transform->GetOrigin());
                SDL_FRect origRectScreen = SDL_FRect{origScreen.x - 2, origScreen.y - 2, 4, 4};
                SDL_RenderFillRectF(_renderer, &origRectScreen);
#endif
            }
        }
    }
};