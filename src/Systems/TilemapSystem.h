#pragma once

#include "../stdafx.h"
#include "../GameClasses/EntityManager.h"
#include "../GameClasses/GameCamera.h"

#include "../Utils/DebugUtils.h"


#include "../Components/TilemapComponent.h"

class TilemapSystem {
private:
    SDL_Renderer* _renderer;
    GameCamera* _camera;

    EntityManager* _entityManager;
    
    // std::vector<SDL_Texture*> tilemapTextures;
    
public:
    TilemapSystem(SDL_Renderer* renderer, GameCamera* camera, EntityManager* entityManager) :
            _renderer(renderer),
            _camera(camera),
            _entityManager(entityManager) 
        {
            std::cout << "TilemapSystem() " <<  this << std::endl;
        }

    ~TilemapSystem() {
        std::cout << "~TilemapSystem() " << this << std::endl;
        // for (auto &tx : tilemapTextures) {
        //     SDL_DestroyTexture(tx);
        // }
        // tilemapTextures.clear();
    }

    // void Update(const float& dt) {
    // // nothing
    // }

    void Render() {

        for (auto& ent : _entityManager->GetEntitiesWithComponent<TilemapComponent>()) {
            
            auto tilemap = ent->GetComponent<TilemapComponent>();
            if (!tilemap->GetActive())
                continue;
            
            if (!tilemap->GetTextureCreated()) {
                GenerateTexture(tilemap);
                continue;
            }

            // correct
            auto pos = tilemap->GetTilemapPosition();
            auto size = tilemap->GetTilemapSize();
            auto tsize = tilemap->GetTileSize();

            // std::cout << "Pos:   " << Vec2toString(pos) << std::endl;
            // std::cout << "Size:  " << Vec2toString(size) << std::endl;
            // std::cout << "TSize: " << Vec2toString(tsize) << std::endl;
            
            SDL_FRect dst = SDL_FRect{
                pos.x,
                pos.y, 
                size.x * tsize.x, 
                size.y * tsize.y
            };

            SDL_FRect dst2 = {};
            _camera->RectWorldToScreen(&dst, &dst2);
            
            // std::cout << *tilemap->GetTexture() << std::endl;
            // std::cout << "Pos:   " << RectToString(dst2) << std::endl;
            SDL_RenderCopyF(_renderer, tilemap->GetTexture(), NULL, &dst2);
        }
    }


    void GenerateTexture(TilemapComponent* component)  {

        // SDL_Texture* tx = nullptr;

        auto tilemapData = component->GetTilemapData();

        component->SetTilemapSize(glm::ivec2(tilemapData[0].size(), tilemapData.size()));
        auto tilemapSize = component->GetTilemapSize();
        auto tileSize = component->GetTileSize();

        auto tilemapLayout = component->GetTilemapLayout();

        // print
        // for (int j = 0; j < _tilemapSize.y; j++) {
        //     for (int i = 0; i < _tilemapSize.x; i++) {
        //         std::cout << tilemapData[j][i] << " ";
        //     }
        //     std::cout << std::endl;
        // }


        component->SetTilemapTexture(SDL_CreateTexture(
            _renderer, 
            SDL_PIXELFORMAT_RGBA8888, 
            SDL_TEXTUREACCESS_TARGET, 
            tilemapSize.x * tileSize.x, 
            tilemapSize.y * tileSize.y));

        auto tx = component->GetTexture();

        SDL_SetTextureBlendMode(tx, SDL_BLENDMODE_BLEND);

        SDL_SetRenderTarget(_renderer, tx);

        for (int y = 0; y < tilemapSize.y; y++) {
            for (int x = 0; x < tilemapSize.x; x++) {

                SDL_FRect dst = SDL_FRect{tileSize.x * x, tileSize.y * y, tileSize.x, tileSize.y};

                int idx = tilemapData[y][x];
                SDL_RenderCopyF(_renderer, tx, &tilemapLayout[idx], &dst);
            }
        }

        SDL_SetRenderTarget(_renderer, NULL);

        component->SetCreated();
    }
};