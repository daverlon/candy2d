#pragma once

#include "stdafx.h"

#include "sprite.h"

struct Camera {
    glm::vec2 position = glm::vec2(0.0f,0.0f);
    glm::vec2 size = glm::vec2(0.0f,0.0f);
    glm::vec2 origin = glm::vec2(0.0f,0.0f); // origin (probably shuold be center of camera)
    float orig_zoom = 4.0f;
    float zoom = 0.0f;
};


namespace g {
    extern Camera camera;
}


extern void MoveCamera(Camera* cam, int x, int y);


extern void DestroyCamera(Camera* cam);

extern void SetCameraPosition(Camera *cam, glm::vec2 size);

extern void SetCameraViewSize(Camera *cam, glm::vec2 size);

extern void SlowlyMoveCamera(Camera *cam, glm::vec2 targetPos, float speed);
