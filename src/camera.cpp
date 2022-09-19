#include "camera.h"

void MoveCamera(Camera* cam, int x, int y) {
    cam->view.x += x;
    cam->view.y += y;
}


void SetCameraFocus(Camera* cam, Sprite* spr) {
    cam->focus = spr;
}

void DestroyCamera(Camera *cam) {
    delete cam;
}

void SetCameraPosition(Camera *cam, int x, int y) {
    cam->view.x = x;
    cam->view.y = y;
}

