#pragma once

#include "../stdafx.h"
#include "BaseComponent.h"

class TilemapComponent : public Component {
private:
    SDL_Texture* _tilemapTexture; // tilemap system holds textures
    // possible improvements in performance due to cpu caching

    std::vector<SDL_Rect> _tilemapLayout; // contains the rect positions (indexed by tilemap data)
    std::vector<std::vector<int>> _tilemapData; // indices for the tilemap layout positions

    glm::vec2 _tilemapPosition; // starting position for the tilemap (world position)
    glm::vec2 _tileSize;
    glm::ivec2 _tilemapSize;

    bool _textureCreated;

public:
    // don't get the reference to the original vectors,
    // store them under this component
    // copy the data

    TilemapComponent(
        std::vector<SDL_Rect> tilemapLayout, 
        std::vector<std::vector<int>> tilemapData,
        glm::ivec2 worldPosition = glm::ivec2(0, 0),
        glm::ivec2 tileSize = glm::ivec2(0, 0)
        )
        :
        // _tilemapTexture(nullptr),

        _tilemapLayout(tilemapLayout),
        _tilemapData(tilemapData),
        _tilemapPosition(worldPosition),
        _tileSize(tileSize),
        _tilemapSize(glm::vec2(0.0f, 0.0f)),
        _textureCreated(false)
    {
        std::cout << "        TilemapComponent() " << this << std::endl;
    }

    ~TilemapComponent() {
        std::cout << "        ~TilemapComponent() " << this << std::endl;
        if (_textureCreated) {
            SDL_DestroyTexture(_tilemapTexture);
            std::cout << "         Destroyed Tilemap texture. " << std::endl;
        }
    }

    // const SDL_Texture* GetTexture() const { return _tilemapTexture; }

    inline const bool GetTextureCreated() const { return _textureCreated; }
    inline void SetCreated(bool val = true) { _textureCreated = val; }

    inline const std::vector<SDL_Rect>& GetTilemapLayout() const { return _tilemapLayout; }
    inline const std::vector<std::vector<int>>& GetTilemapData() const { return _tilemapData; }

    inline const glm::vec2 GetTilemapPosition() const { return _tilemapPosition; }
    inline const glm::vec2 GetTileSize() const { return _tileSize; }
    inline const glm::vec2 GetTilemapSize() const { return _tilemapSize; }

    inline void SetTilemapSize(glm::ivec2 size) { _tilemapSize = size; }

    inline SDL_Texture* GetTexture() { return _tilemapTexture; }

    inline void SetTilemapTexture(SDL_Texture* texture) { _tilemapTexture = texture; }
};