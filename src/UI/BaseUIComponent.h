#pragma once

#include "../stdafx.h"

class BaseUIComponent {
private:
    glm::ivec2 _position;  // screen position
    glm::ivec2 _size;      // screen size
    bool _active;

public:
    BaseUIComponent() : _position(glm::ivec2(0, 0)), _size(glm::ivec2(0, 0)), _active(true) {

    }

    virtual ~BaseUIComponent();

    virtual void Render(SDL_Renderer* renderer);
};