#include "camera.h"

void MoveCamera(Camera* cam, int x, int y) {
    cam->position.x += x;
    cam->position.y += y;
}


void DestroyCamera(Camera *cam) {
    delete cam;
}

void SetCameraPosition(Camera *cam, glm::vec2 position) {
    cam->position = position;
}

void SetCameraViewSize(Camera *cam, glm::vec2 size) {
    cam->view_size = size;
}


void SlowlyMoveCamera(Camera *cam, glm::vec2 targetPos, float speed=5.0f) {

    glm::vec2 delta = targetPos - cam->position;
    std::cout << "delta: ";
    PrintVec2(delta);
    cam->position.x += delta.x*speed;
    cam->position.y += delta.y*speed;
    PrintVec2(cam->position);
}
