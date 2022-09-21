#pragma once

#include "stdafx.h"

#include <iostream>
#include <vector>

struct Sprite {
    Sprite(SDL_Texture* t, SDL_Rect src_rect, glm::vec2 origin) {
        this->texture = t;
        this->src_rect = src_rect;
        this->origin = origin;
        this->position = glm::vec2(0.0f,0.0f);
    }

    SDL_Texture* texture;
    SDL_Rect src_rect;
    glm::vec2 position;
    glm::vec2 origin;
};



extern void SetSpriteOrigin(Sprite* spr, glm::vec2 origin);


extern void MoveSprite(Sprite *spr, glm::vec2 pos);


extern void SetSpritePosition(Sprite *spr, glm::vec2 pos);


extern int FindSpriteIndex(std::vector<Sprite*> sprites, Sprite* search);


extern void DestroyAllSprites(std::vector<Sprite*> *sprites);


extern void DestroySprite(std::vector<Sprite*>* sprites, int index);

