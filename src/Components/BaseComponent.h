#pragma once

//#include "../stdafx.h"

class Component {
private:
    bool _active;
public:
    Component(bool active = true) :
        _active(active) {
            // std::cout << "    Component()" << std::endl;
        }

    virtual ~Component() {
        // std::cout << "    ~Component()" << std::endl;
    };

    bool GetActive() const { return _active; }
    void SetActive(bool active) { _active = active; }
};