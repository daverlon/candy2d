#pragma once

#include "sdl2sdk.h"



struct TileMap {
        glm::vec2 position; // top left 
        glm::vec2 tile_size;
        std::string atlast_path;
        std::string tilemap_string;

        TileMap(glm::vec2 tile_size, std::string atlas_path, std::string data) {
            this->position = glm::vec2(0.0f, 0.0f);
            this->tile_size = tile_size;
            this->atlast_path = atlas_path;
            this->tilemap_string = data;
        }

    SDL_Texture* texture; 
};