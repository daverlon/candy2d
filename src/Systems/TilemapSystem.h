#pragma once

#include "../stdafx.h"
#include "../GameClasses/EntityManager.h"
#include "../GameClasses/GameCamera.h"

#include "../Utils/DebugUtils.h"


#include "../Components/TilemapComponent.h"

// todo? :
// tilemap layers (essentially just rendering over the texture for different layers)
// tilemap blank spaces (perhaps if the index is -1, it skips, leaving a blank area in between tiles)

class TilemapSystem {
private:
    SDL_Renderer* _renderer;
    EntityManager* _entityManager;

    SDL_Texture* _tileSetTexture;

    GameCamera* _camera;

    
    // std::vector<SDL_Texture*> tilemapTextures;
    
public:
    TilemapSystem(SDL_Renderer* renderer, EntityManager* entityManager, SDL_Texture* tileSetTexture, GameCamera* camera) :
            _renderer(renderer),
            _entityManager(entityManager),
            _tileSetTexture(tileSetTexture),
            _camera(camera)
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

    void Render();

    void GenerateTexture(TilemapComponent* component);

};