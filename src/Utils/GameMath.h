#pragma once

#include "../stdafx.h"

extern inline void ClampVec2(glm::vec2* v, const SDL_FRect *bounds) {
    if (v->x > (bounds->x + bounds->w)) v->x = (bounds->x + bounds->w);
    if (v->x < (bounds->x)) v->x = bounds->x;
    if (v->y > (bounds->y + bounds->h)) v->y = (bounds->y + bounds->h);
    if (v->y < (bounds->y)) v->y = bounds->y;
}

extern inline void ClampVec2(glm::ivec2* v, const SDL_Rect *bounds) {
    if (v->x > (bounds->x + bounds->w)) v->x = (bounds->x + bounds->w);
    if (v->x < (bounds->x)) v->x = bounds->x;
    if (v->y > (bounds->y + bounds->h)) v->y = (bounds->y + bounds->h);
    if (v->y < (bounds->y)) v->y = bounds->y;
}

extern inline void ClampVec2(glm::ivec2* v, const SDL_FRect *bounds) {
    if (v->x > (bounds->x + bounds->w)) v->x = (bounds->x + bounds->w);
    if (v->x < (bounds->x)) v->x = bounds->x;
    if (v->y > (bounds->y + bounds->h)) v->y = (bounds->y + bounds->h);
    if (v->y < (bounds->y)) v->y = bounds->y;
}

extern inline SDL_FRect Vec2Vec2toRect(const glm::vec2& v1, const glm::vec2& v2) {
    return (SDL_FRect){v1.x, v1.y, v2.x, v2.y};
}

extern inline SDL_FRect Vec2Vec2toRect(const glm::vec2& v1, const glm::ivec2& v2) {
    return (SDL_FRect){v1.x, v1.y, (float)v2.x, (float)v2.y};
}

extern inline SDL_Rect Vec2Vec2toRect(const glm::ivec2& v1, const glm::ivec2& v2) {
    return (SDL_Rect){v1.x, v1.y, v2.x, v2.y};
}