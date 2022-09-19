#pragma once

#include "sdl2sdk.h"

#include "sprite.h"

struct Camera {
    Camera(SDL_Rect view, int zoom, Sprite* focus) {
        this->view = view;
        this->zoom = zoom;
        this->focus = focus;
    }
    Camera(SDL_Rect view, int zoom) {
        this->view = view;
        this->zoom = zoom;
    }
    SDL_Rect view;
    int zoom;

    Sprite* focus;
};


extern void MoveCamera(Camera* cam, int x, int y);

extern void SetCameraFocus(Camera* cam, Sprite* spr);

extern void DestroyCamera(Camera* cam);

extern void SetCameraPosition(Camera *cam, int x, int y);