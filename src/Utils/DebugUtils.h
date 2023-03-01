#pragma once

#include "../stdafx.h"

extern inline void ShowError(const std::string &msg) {
    std::cout << msg << std::endl;
    std::cout << SDL_GetError() << std::endl;
}

extern inline std::string Vec2toString(const glm::vec2 &v) {
    return "(" + std::to_string(v.x) + "," + std::to_string(v.y) + ")";
}

extern inline std::string Vec2toString(const glm::ivec2 &v) {
    return "(" + std::to_string(v.x) + "," + std::to_string(v.y) + ")";
}

extern inline std::string RectToString(const SDL_Rect &r) {
    return "(" + std::to_string(r.x) + "," + std::to_string(r.y) + "," + std::to_string(r.w) + "," + std::to_string(r.h) + ")";
}