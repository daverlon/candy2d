#include "gamemath.h"


glm::vec2 WorldToScreen(const Sprite* spr, const ViewPort *view, const Camera *cam) {
    glm::vec2 ret = glm::vec2(0,0);

    ret += g::viewport.position;
    ret += cam->position + (cam->size/2.0f);
    ret -= spr->position;
    ret -= (spr->origin * g::camera.zoom); // adjust origin by camera zoom!

    return ret;
}

glm::vec2 WorldToScreen(const glm::vec2* pos, const ViewPort *view, const Camera *cam) {
    glm::vec2 ret = glm::vec2(0.0f,0.0f);

    ret += g::viewport.position;
    ret += cam->position + (cam->size/2.0f);
    ret -= *pos; // assuming that pos is the origin

    return ret;
}




