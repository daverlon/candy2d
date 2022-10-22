#include "tilemap.h"

#include "stdafx.h"
#include "debugutils.h"

glm::ivec2 MatchTile(int id, std::vector<Tile> tiles) {
    int l = tiles.size();
    for (int i = 0; i < l; i++) {
        if (tiles[i].id == id) return tiles[i].coord;
    }
    return glm::ivec2(0,0);
}

void LoadTileMap(SDL_Texture **to, std::string atlas_path, std::string atlas_layout, std::string tilemap_layout, glm::ivec2 tile_size) {
    // std::string tilemap_atlas_path = "res/img/test_tilemap.png";
    // glm::ivec2 tile_size = glm::ivec2(32, 32);
    // std::string atlas_layout = "1,2,n,3,4,";


    // setup atlas texture
    SDL_Texture* atlas_texture = IMG_LoadTexture(g::renderer, atlas_path.c_str());
    glm::ivec2 atlas_size = glm::ivec2(0, 0);
    SDL_QueryTexture(atlas_texture, NULL, NULL, &atlas_size.x, &atlas_size.y);
    std::cout << "Loaded atlas: " << atlas_path << ", size: " << Vec2toString(atlas_size) << std::endl;

    // ---------------------------- //
  
    // buffers
    std::vector<Tile> coords;
    glm::ivec2 cur_coord = glm::ivec2(0,0);
    std::string num_buff = "";

    // load tile data
    for (char &c : atlas_layout) {
        if (c == 'n') {
            cur_coord.x = -tile_size.x;
            cur_coord.y += tile_size.y;
            continue;
        }
        if (c == ',') {
            if (num_buff != "") {
                std::cout << num_buff << "," << Vec2toString(cur_coord) << std::endl;
                coords.push_back((Tile){std::stoi(num_buff), cur_coord});
            }
            cur_coord.x += tile_size.x;
            num_buff.clear();
            continue;
        }
        num_buff += c;
    }
    std::cout << std::endl;

    // render layout to texture
    glm::ivec2 coord = glm::ivec2(0,0);
    int id = 2;   // (0,32)

    glm::ivec2 tilemap_size = glm::ivec2(64, 64);
    SDL_SetRenderTarget(g::renderer, *to);

    cur_coord = glm::ivec2(0,0);
    for (char &c : tilemap_layout) {
        if (c == 'n') {
            cur_coord.x = -tile_size.x;
            cur_coord.y += tile_size.y;
            continue;
        }
        if (c == ',') {
            if (num_buff != "") {

                glm::ivec2 pos = MatchTile(std::stoi(num_buff), coords);
                SDL_Rect r = (SDL_Rect){pos.x, pos.y, 32, 32};
                SDL_Rect r2 = (SDL_Rect) {cur_coord.x, cur_coord.y, 32, 32};
                std::cout << "Rendering tile " << std::stoi(num_buff)
                << " " << Vec2toString(pos) << " to " << Vec2toString(cur_coord) << std::endl;
                SDL_RenderCopy(g::renderer, atlas_texture, &r, &r2);
            }
            cur_coord.x += tile_size.x;
            num_buff.clear();
            continue;
        }
        num_buff += c;
    }   
    std::cout << "Tilemap created" << std::endl;
}