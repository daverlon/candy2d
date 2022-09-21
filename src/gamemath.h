#pragma once

#include "stdafx.h"

#include "sprite.h"
#include "viewport.h"
#include "camera.h"


extern glm::vec2 WorldToScreen(const Sprite* spr, const ViewPort *view, const Camera *cam);


extern glm::vec2 WorldToScreen(const glm::vec2* pos, const ViewPort *view, const Camera *cam);


extern glm::vec2 WorldToScreen(const glm::ivec2* pos, const ViewPort *view, const Camera *cam);


extern void ClampVec2(glm::vec2* v, const SDL_Rect* bounds);

extern void ClampVec2(glm::ivec2* v, const SDL_Rect* bounds);


extern SDL_Rect Vec2Vec2toRect(const glm::vec2* v1, const glm::vec2* v2);


extern SDL_Rect Vec2Vec2toRect(const glm::vec2* v1, const glm::ivec2* v2);


extern SDL_Rect Vec2Vec2toRect(const glm::ivec2* v1, const glm::ivec2* v2);

