#pragma once

#include "../stdafx.h"
#include "../Utils/GameMath.h"

class GameViewPort {
private:
    glm::ivec2 _size;
    glm::vec2 _position;
    glm::ivec2 _mousePosition;

public:   
    GameViewPort(glm::ivec2 size = glm::vec2(0.0f, 0.0f), glm::vec2 position = glm::ivec2(0, 0)) : 
        _size(size), _position(position), _mousePosition(glm::ivec2(0, 0)) {}

    inline glm::vec2 GetSize() const { return _size; }
    inline void SetSize(glm::ivec2 size)  { _size = size; }

    inline glm::vec2 GetPosition() const { return _position; }
    inline void SetPosition(glm::vec2 pos) { _position = pos; }

    inline glm::ivec2 GetMousePosition() const { return _mousePosition; }
    inline void SetMousePosition(glm::ivec2 pos) { _mousePosition = pos; }

    inline void UpdateMousePosition() { 
        glm::ivec2 mouse_position_buffer = glm::ivec2(0, 0);
        SDL_GetMouseState(&mouse_position_buffer.x, &mouse_position_buffer.y);
        SDL_FRect bounds = Vec2Vec2toRect(GetPosition(), GetSize());
        ClampVec2(&mouse_position_buffer, &bounds);
        mouse_position_buffer -= GetPosition();
        SetMousePosition(mouse_position_buffer);
    }

    inline bool IsMouseInViewPort() {
        glm::vec2 cursor_area = glm::ivec2(2, 2);
        glm::vec2 adjusted_mousepos = GetPosition();
        adjusted_mousepos += GetMousePosition();
        SDL_FRect mouse_rect = Vec2Vec2toRect(adjusted_mousepos, cursor_area);
        SDL_FRect viewport_rect = Vec2Vec2toRect(GetPosition(), GetSize());
        return SDL_HasIntersectionF(&mouse_rect, &viewport_rect);
    }

    // void CorrectViewPortSize() {
    //     SDL_GetWindowSize(g::window, &g::window_size.x, &g::window_size.y);
    //     //std::cout << "New window size: " << Vec2toString(g::window_size) << std::endl;
    //     if (g::window_size.x > g::window_size.y) {
    //         g::viewport.size = glm::ivec2(g::window_size.y, g::window_size.y);
    //         g::viewport.position.x = (g::window_size.x/2) - (g::viewport.size.x/2);
    //         g::viewport.position.y = 0;
    //         g::camera.size = g::viewport.size;
    //     }
    //     else if (g::window_size.x <= g::window_size.y) {
    //         g::viewport.size = glm::ivec2(g::window_size.x, g::window_size.x);
    //         g::viewport.position.y = (g::window_size.y/2) - (g::viewport.size.y/2);
    //         g::viewport.position.x = 0;
    //         g::camera.size = g::viewport.size;
    //     }
    // }
};