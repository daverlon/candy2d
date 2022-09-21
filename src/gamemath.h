#pragma once

#include "stdafx.h"

#include "sprite.h"
#include "viewport.h"
#include "camera.h"


glm::vec2 WorldToScreen(const Sprite* spr, const ViewPort *view, const Camera *cam);


glm::vec2 WorldToScreen(const glm::vec2* pos, const ViewPort *view, const Camera *cam);