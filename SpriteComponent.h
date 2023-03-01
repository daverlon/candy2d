#pragma once

#include "../stdafx.h"
#include "BaseComponent.h"

class SpriteComponent : public Component {
private:
    SDL_Rect _srcRect; // assumes that the texture is on a pre determined texture atlas
public:
    SpriteComponent(SDL_Rect srcRect = SDL_Rect{0, 0, 0, 0}) : 
        _srcRect(srcRect) {
            std::cout << "        SpriteComponent()" << this << std::endl;
        }

    ~SpriteComponent() {
        std::cout << "        ~SpriteComponent()" << this << std::endl;
    }

    const SDL_Rect& GetSrcRect() const { return _srcRect; }    
    void SetSrcRect(SDL_Rect srcRect) { _srcRect = srcRect; }

    void SetSrcRectX(int x) {
        _srcRect.x = x;
    }
};