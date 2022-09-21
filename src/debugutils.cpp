#include "debugutils.h"


void ShowError(const std::string &msg) {
    std::cout << msg << std::endl;
    std::cout << SDL_GetError() << std::endl;
}

std::string Vec2toString(const glm::vec2 &v) {
    return "(" + std::to_string(v.x) + "," + std::to_string(v.y) + ")";
}

std::string Vec2toString(const glm::ivec2 &v) {
    return "(" + std::to_string(v.x) + "," + std::to_string(v.y) + ")";
}