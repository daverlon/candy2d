#pragma once

#include "sdl2sdk.h"

#include <iostream>
#include <vector>

struct Sprite {
    Sprite(SDL_Texture* t, SDL_Rect img_rect, int origin_x, int origin_y) {
        this->texture = t;
        this->img_rect = img_rect;
        this->origin_x = origin_x;
        this->origin_y = origin_y;
        this->img_pos = (SDL_Rect){0,0,this->img_rect.w,this->img_rect.h};
    }

    SDL_Texture* texture;
    SDL_Rect img_rect;
    SDL_Rect img_pos;
    int origin_x;
    int origin_y;
};


extern void SetSpriteOrigin(Sprite* spr, int x, int y);


extern void MoveSprite(Sprite *spr, int x, int y);


extern int FindSpriteIndex(std::vector<Sprite*> sprites, Sprite* search);


extern void DestroyAllSprites(std::vector<Sprite*> *sprites);


extern void DestroySprite(std::vector<Sprite*>* sprites, int index);