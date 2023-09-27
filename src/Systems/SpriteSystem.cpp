#include "SpriteSystem.h"

void SpriteSystem::Render() {

    // if (_needsLayerUpdate) {
        UpdateEntitiesByLayer();
    // }

    YSort();

    // Render sprites by layer
    for (const auto& layerEntry : _entitiesByLayer) {
        const std::vector<Entity*>& entities = layerEntry.second;

        for (Entity* entity : entities) {
            SpriteComponent* sprite = entity->GetComponent<SpriteComponent>();
            TransformComponent* transform = entity->GetComponent<TransformComponent>();
            assert(sprite != nullptr && transform != nullptr);

            glm::vec2 world_pos = transform->GetPosition();

            SDL_Rect sprite_rect = sprite->GetSrcRect();
            SDL_FRect sprite_bounds_world = SDL_FRect{world_pos.x, world_pos.y, (float)sprite_rect.w, (float)sprite_rect.h};
            SDL_FRect sprite_bounds_screen = SDL_FRect{};
            _camera->RectWorldToScreen(&sprite_bounds_world, &sprite_bounds_screen);

            if (sprite->GetFlipped())
                SDL_RenderCopyExF(_renderer, _tileSetTexture, &sprite->GetSrcRect(), &sprite_bounds_screen, sprite->GetAngle(), NULL, SDL_FLIP_HORIZONTAL);
            else
                SDL_RenderCopyExF(_renderer, _tileSetTexture, &sprite->GetSrcRect(), &sprite_bounds_screen, sprite->GetAngle(), NULL, SDL_FLIP_NONE);

#ifdef DEBUG
            // render bounds (debug)
            SDL_SetRenderDrawColor(_renderer, 255, 0, 255, 255);
            SDL_RenderDrawRectF(_renderer, &sprite_bounds_screen);



            // render origin (debug)
            SDL_SetRenderDrawColor(_renderer, 0, 255, 0, 255);
            glm::vec2 origScreen = _camera->WorldToScreen(world_pos + transform->GetOrigin());
            SDL_FRect origRectScreen = SDL_FRect{origScreen.x - 2, origScreen.y - 2, 4, 4};
            SDL_RenderFillRectF(_renderer, &origRectScreen);
#endif
        }
    }
}