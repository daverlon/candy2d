#pragma once

#include "stdafx.h"

struct TileMap {
    TileMap(glm::ivec2 tilemapSize) {
        texture = SDL_CreateTexture(
            g::renderer, 
            SDL_PIXELFORMAT_RGBA8888, 
            SDL_TEXTUREACCESS_TARGET,
            tilemapSize.x,
            tilemapSize.y
        );
    }
    glm::ivec2 position;
    SDL_Texture *texture; // final tilemap texture
};

struct Tile {
    int id;
    glm::ivec2 coord;
};

extern void LoadTileMap(SDL_Texture **to, std::string atlas_path, std::string atlas_layout, std::string tilemap_layout, glm::ivec2 tile_size);