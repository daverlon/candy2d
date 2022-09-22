#include "camera.h"

void MoveCamera(Camera* cam, int x, int y) {
    cam->position.x += x;
    cam->position.y += y;
}


void DestroyCamera(Camera *cam) {
    delete cam;
}

void SetCameraPosition(Camera *cam, glm::vec2 position) {
    cam->position = position * (g::camera.zoom/g::camera.orig_zoom);
}

void SetCameraViewSize(Camera *cam, glm::vec2 size) {
    cam->size = size;
}


void SlowlyMoveCamera(Camera *cam, glm::vec2 targetPos, float speed=5.0f) {

    glm::vec2 target = targetPos * (g::camera.zoom/g::camera.orig_zoom);
    glm::vec2 delta = target - (cam->position);
    //std::cout << "delta: " << Vec2toString(delta) << std::endl;
    cam->position.x += delta.x*speed;
    cam->position.y += delta.y*speed;
    //std::cout << "delta: " << Vec2toString(cam->position) << std::endl;
}

