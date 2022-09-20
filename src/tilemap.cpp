
#include "tilemap.h"



std::string example = "1 1 1 1 1 1 1 1 1 1 1";



void InitTileMapTexture(SDL_Renderer* renderer, TileMap* tilemap) {
    SDL_Texture* atlas_texture = CTFS(renderer, IMG_Load(tilemap->atlast_path.c_str()));

    tilemap->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 800, 800);
    SDL_SetRenderTarget(renderer, tilemap->texture);
}