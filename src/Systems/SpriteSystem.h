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

    inline void UpdateEntitiesByLayer() {
        _entitiesByLayer.clear();

        for (auto& entity : _entityManager->GetEntitiesWithComponent<SpriteComponent>()) {
            SpriteComponent* sprite = entity->GetComponent<SpriteComponent>();
            assert(sprite != nullptr);
            if (!sprite->GetActive()) continue;

            _entitiesByLayer[sprite->GetLayer()].push_back(entity);
        }
        _needsLayerUpdate = false;
    }

    inline void YSort() {
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

    void Render();

};