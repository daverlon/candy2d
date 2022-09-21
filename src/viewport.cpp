#include "viewport.h"

#include "gamemath.h"


bool MouseIsInViewPort(const ViewPort &viewport) {
    glm::ivec2 cursor_area = glm::ivec2(2,2);
    glm::ivec2 adjustedmousepos = viewport.position;
    adjustedmousepos += viewport.mouse_position;
    SDL_Rect mouse_rect = Vec2Vec2toRect(&adjustedmousepos, &cursor_area);
    SDL_Rect viewport_rect = Vec2Vec2toRect(&viewport.position, &viewport.size);
    return SDL_HasIntersection(&mouse_rect, &viewport_rect);
}