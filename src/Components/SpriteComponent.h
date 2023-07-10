#pragma once

#include "../stdafx.h"
#include "BaseComponent.h"

class SpriteComponent : public Component {
private:
    SDL_Rect _srcRect; // assumes that the texture is on a pre determined texture atlas
    int _spriteLayer;
    bool _flipped;
public:
    SpriteComponent(SDL_Rect srcRect = SDL_Rect{0, 0, 0, 0}, int spriteLayer = SPRITE_LAYER_YSORT) : 
        _srcRect(srcRect), _spriteLayer(spriteLayer), _flipped(false) {
            std::cout << "        SpriteComponent()" << this << std::endl;
        }

    ~SpriteComponent() {
        std::cout << "        ~SpriteComponent()" << this << std::endl;
    }

    const SDL_Rect& GetSrcRect() const { return _srcRect; }    
    void SetSrcRect(SDL_Rect srcRect) { _srcRect = srcRect; }

    inline void SetSrcRectX(int x) {
        _srcRect.x = x;
    }

    inline int GetLayer() { return _spriteLayer; }

    inline bool GetFlipped() { return _flipped; }
    inline void Flip() { _flipped = !_flipped; }
};