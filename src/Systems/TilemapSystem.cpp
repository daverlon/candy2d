#include "TilemapSystem.h"

void TilemapSystem::Render() {

    auto ents = _entityManager->GetEntitiesWithComponent<TilemapComponent>();
    std::reverse(ents.begin(), ents.end());
    
    // reverse the list 
    for (auto& ent : ents) {
        
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


void TilemapSystem::GenerateTexture(TilemapComponent* component)  {

    // SDL_Texture* tx = nullptr;

    auto tilemapData = component->GetTilemapData();

    component->SetTilemapSize(glm::ivec2(tilemapData[0].size(), tilemapData.size()));
    auto tilemapSize = component->GetTilemapSize();
    auto tileSize = component->GetTileSize();

    auto tilemapLayout = component->GetTilemapLayout();

    // print
    for (int j = 0; j < tilemapSize.y; j++) {
        for (int i = 0; i < tilemapSize.x; i++) {
            std::cout << tilemapData[j][i] << " ";
        }
        std::cout << std::endl;
    }


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

            int idx = tilemapData[y][x];
            if (idx == -1) continue;

            SDL_FRect dst = SDL_FRect{tileSize.x * x, tileSize.y * y, tileSize.x, tileSize.y};

            SDL_RenderCopyF(_renderer, _tileSetTexture, &tilemapLayout[idx], &dst);
        }
    }

    SDL_SetRenderTarget(_renderer, NULL);

    component->SetCreated();
}