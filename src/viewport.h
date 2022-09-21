#pragma once

#include "stdafx.h"


struct ViewPort {
    glm::ivec2 size; // size of camera view
    glm::vec2 position; // position (top left)

    // mouse position (starting from viewport position)
    // on-screen mouse position
    glm::ivec2 mouse_position; 

    // final texture to be rendered by SDL2
    //SDL_Texture *output_texture;
};

namespace g {
    extern ViewPort viewport;
}