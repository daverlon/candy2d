#include "viewport.h"

#include "gamemath.h"


bool MouseIsInViewPort(const ViewPort &viewport) {
    glm::vec2 cursor_area = glm::ivec2(2,2);
    glm::vec2 adjustedmousepos = viewport.position;
    adjustedmousepos += viewport.mouse_position;
    SDL_FRect mouse_rect = Vec2Vec2toRect(adjustedmousepos, cursor_area);
    SDL_FRect viewport_rect = Vec2Vec2toRect(viewport.position, viewport.size);
    return SDL_HasIntersectionF(&mouse_rect, &viewport_rect);
}

// only supports 1:1 ratio
// todo: 16:9 ratios
void CorrectViewPortSize() {
    SDL_GetWindowSize(g::window, &g::window_size.x, &g::window_size.y);
    //std::cout << "New window size: " << Vec2toString(g::window_size) << std::endl;
    if (g::window_size.x > g::window_size.y) {
        g::viewport.size = glm::ivec2(g::window_size.y, g::window_size.y);
        g::viewport.position.x = (g::window_size.x/2) - (g::viewport.size.x/2);
        g::viewport.position.y = 0;
        g::camera.size = g::viewport.size;
    }
    else if (g::window_size.x <= g::window_size.y) {
        g::viewport.size = glm::ivec2(g::window_size.x, g::window_size.x);
        g::viewport.position.y = (g::window_size.y/2) - (g::viewport.size.y/2);
        g::viewport.position.x = 0;
        g::camera.size = g::viewport.size;
    }
}

void UpdateMousePosition(ViewPort &v) {
    glm::ivec2 mouse_position_buffer = glm::ivec2(0,0);
    SDL_GetMouseState(&mouse_position_buffer.x, &mouse_position_buffer.y);
    SDL_FRect bounds = Vec2Vec2toRect(g::viewport.position, g::viewport.size);
    ClampVec2(&mouse_position_buffer, &bounds);
    // onscreen mouse position
    g::viewport.mouse_position = mouse_position_buffer;
    g::viewport.mouse_position -= g::viewport.position;
}