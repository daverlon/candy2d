#include "gamemath.h"


glm::vec2 WorldToScreen(const Sprite* spr, const ViewPort *view, const Camera *cam) {
    glm::vec2 ret = glm::vec2(0,0);
    // ret += g::viewport.position;
    ret += cam->position + (cam->size/2.0f);
    ret -= spr->position;
    ret -= (spr->origin * g::camera.zoom); // adjust origin by camera zoom!

    return ret;
}

glm::vec2 WorldToScreen(const glm::vec2* pos, const ViewPort *view, const Camera *cam) {
    glm::vec2 ret = glm::vec2(0.0f,0.0f);
    // ret += g::viewport.position;
    ret += cam->position + (cam->size/2.0f);
    ret -= *pos; // assuming that pos is the origin
    return ret;
}

glm::vec2 WorldToScreen(const glm::ivec2* pos, const ViewPort *view, const Camera *cam) {
    glm::vec2 ret = glm::vec2(0.0f,0.0f);
    // ret += g::viewport.position;
    ret += cam->position + (cam->size/2.0f);
    ret -= *pos; // assuming that pos is the origin
    return ret;
}

glm::vec2 ScreenToWorld(const glm::ivec2* pos, const ViewPort *view, const Camera *cam) {
    glm::vec2 ret = glm::vec2(0.0f, 0.0f);
    // todo
}

void ClampVec2(glm::vec2* v, const SDL_Rect *bounds) {
    if (v->x > (bounds->x + bounds->w)) v->x = (bounds->x + bounds->w);
    if (v->x < (bounds->x)) v->x = bounds->x;
    if (v->y > (bounds->y + bounds->h)) v->y = (bounds->y + bounds->h);
    if (v->y < (bounds->y)) v->y = bounds->y;
}

void ClampVec2(glm::ivec2* v, const SDL_Rect *bounds) {
    if (v->x > (bounds->x + bounds->w)) v->x = (bounds->x + bounds->w);
    if (v->x < (bounds->x)) v->x = bounds->x;
    if (v->y > (bounds->y + bounds->h)) v->y = (bounds->y + bounds->h);
    if (v->y < (bounds->y)) v->y = bounds->y;
}

SDL_Rect Vec2Vec2toRect(const glm::vec2* v1, const glm::vec2* v2) {
    return (SDL_Rect){v1->x, v1->y, v2->x, v2->y};
}

SDL_Rect Vec2Vec2toRect(const glm::vec2* v1, const glm::ivec2* v2) {
    return (SDL_Rect){v1->x, v1->y, v2->x, v2->y};
}

SDL_Rect Vec2Vec2toRect(const glm::ivec2* v1, const glm::ivec2* v2) {
    return (SDL_Rect){v1->x, v1->y, v2->x, v2->y};
}