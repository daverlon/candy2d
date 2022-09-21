#pragma once

#include "stdafx.h"


struct ViewPort {
    glm::ivec2 size = glm::vec2(0,0); // size of camera view
    glm::vec2 position = glm::ivec2(0, 0); // position (top left)

    // mouse position (starting from viewport position)
    // on-screen mouse position
    glm::ivec2 mouse_position = glm::vec2(0,0); 

    // final texture to be rendered by SDL2
    //SDL_Texture *output_texture;
};

namespace g {
    extern ViewPort viewport;
}


extern bool MouseIsInViewPort(const ViewPort &viewport);