#pragma once


#include <iostream>
#include <vector>
#include <string>
#include <format>
#include <algorithm>


#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

//#define CTFS SDL_CreateTextureFromSurface


#include <glm/vec2.hpp>


namespace g {
    extern SDL_Window* window;
    extern SDL_Renderer* renderer;
    extern glm::ivec2 window_size;
}


extern void PrintVec2(glm::vec2 v);

