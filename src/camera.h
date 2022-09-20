#pragma once

#include "sdl2sdk.h"

#include "sprite.h"

struct Camera {
    glm::vec2 position;
    glm::vec2 view_size;
    float zoom;
    SDL_Rect inner_margin;

    Camera(glm::vec2 position, glm::vec2 view_size, float zoom) {
        this->position = position;
        this->view_size = view_size;
        this->zoom = zoom;
    }
};


extern void MoveCamera(Camera* cam, int x, int y);


extern void DestroyCamera(Camera* cam);

extern void SetCameraPosition(Camera *cam, glm::vec2 size);

extern void SetCameraViewSize(Camera *cam, glm::vec2 size);

extern void SlowlyMoveCamera(Camera *cam, glm::vec2 targetPos, float speed);