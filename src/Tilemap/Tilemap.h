#pragma once

#include "../stdafx.h"
#include "../GameClasses/GameCamera.h"

class Tilemap {
private:
    SDL_Renderer* _renderer;
    GameCamera* _camera;

    SDL_Texture* _tilesetTexture;

    glm::vec2 _tilemapPosition; // starting position for the tilemap (world position)
    glm::vec2 _tileSize;
    glm::vec2 _tilemapSize;
    // std::vector<SDL_Rect> _tilemapLayout; // contains the rect positions (indexed by tilemap data)

    // std::vector<std::vector<int>> _tilemapData; // indices for the tilemap layout positions

    SDL_Texture* _tilemapTexture; // actaul texture for the tilemap

public:
    Tilemap(SDL_Renderer* renderer,
            GameCamera* camera,
            SDL_Texture* tilesetTexture, 
            glm::vec2 tilemapPosition, 
            glm::vec2 tileSize
            // const std::vector<std::vector<int>>& tilemapData,
            // std::vector<SDL_Rect> tilemapLayout
        ) : 
        _renderer(renderer),
        _camera(camera),
        _tilesetTexture(tilesetTexture), 
        _tilemapPosition(tilemapPosition), 
        _tileSize(tileSize),
        _tilemapSize(glm::ivec2(0, 0)),
        // _tilemapLayout(tilemapLayout),
        // _tilemapData(tilemapData),
        _tilemapTexture(nullptr)
         {

            std::cout << "Tilemap(...) " << this << std::endl;

        }

    ~Tilemap() {
        SDL_DestroyTexture(_tilemapTexture);
    }

    // todo: collisions?
    // todo: animated tiles? (may need to use sprite object)

    void GenerateTexture(const std::vector<std::vector<int>>& tilemapData, std::vector<SDL_Rect>& tilemapLayout)  {
        assert(_tilemapTexture == nullptr);

        _tilemapSize = glm::ivec2(tilemapData[0].size(), tilemapData.size());

        // print
        for (int j = 0; j < _tilemapSize.y; j++) {
            for (int i = 0; i < _tilemapSize.x; i++) {
                std::cout << tilemapData[j][i] << " ";
            }
            std::cout << std::endl;
        }

        _tilemapTexture = SDL_CreateTexture(
            _renderer, 
            SDL_PIXELFORMAT_RGBA8888, 
            SDL_TEXTUREACCESS_TARGET, 
            _tilemapSize.x * _tileSize.x, 
            _tilemapSize.y * _tileSize.y);

        SDL_SetTextureBlendMode(_tilemapTexture, SDL_BLENDMODE_BLEND);

        SDL_SetRenderTarget(_renderer, _tilemapTexture);

        for (int y = 0; y < _tilemapSize.y; y++) {
            for (int x = 0; x < _tilemapSize.x; x++) {

                SDL_FRect dst = SDL_FRect{_tileSize.x * x, _tileSize.y * y, _tileSize.x, _tileSize.y};

                int idx = tilemapData[y][x];
                SDL_RenderCopyF(_renderer, _tilesetTexture, &tilemapLayout[idx], &dst);
            }
        }

        SDL_SetRenderTarget(_renderer, NULL);
    }

    void Render() {
        assert(_tilemapTexture != nullptr);

        SDL_FRect dst = SDL_FRect{
            _tilemapPosition.x,
            _tilemapPosition.y, 
            _tilemapSize.x * _tileSize.x, 
            _tilemapSize.y * _tileSize.y
        };

        SDL_FRect dst2 = {};
        _camera->RectWorldToScreen(&dst, &dst2);
        
        SDL_RenderCopyF(_renderer, _tilemapTexture, NULL, &dst2);
    }
};