#pragma once

#include "../stdafx.h"
#include "BaseComponent.h"

class PlayerComponent : public Component {
private:
    //
public:
    PlayerComponent() {
        std::cout << "        PlayerComponent()" << this << std::endl;
    }

    ~PlayerComponent() {
        std::cout << "        ~PlayerComponent()" << this << std::endl;
    }

};