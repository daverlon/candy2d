#pragma once

#include "../stdafx.h"
#include "../Components/BaseComponent.h"


class Entity {
private:
    std::vector<Component*> _components;

public:
    // template<typename... Args>
    // Entity(Args&&... args) {
    //     // do not use this...
    //     AddComponent(new Component(std::forward<Args>(args)...));
    // }

    Entity() {
        std::cout << "Entity() " << this << std::endl;
    }

    ~Entity() {
#ifdef SHOWDEBUG
        std::cout << "Started entity destruct" << std::endl;
        std::cout << "~Entity() " << this << std::endl;
        std::cout << "=======" << std::endl;
        std::cout << "Component list size: " << _components.size() << std::endl;
        std::cout << "=======" << std::endl;
#endif

        for (auto c : _components) {
            delete c;
        } 
        _components.clear(); // avoid double delete
#ifdef SHOWDEBUG
        std::cout << "Component list size: " << _components.size() << std::endl;
        std::cout << "=======" << std::endl;
        std::cout << std::endl;
#endif
    }


    void AddComponent(Component* component) {
        _components.push_back(component);
    }

    template<typename... ComponentPtrs>
    void AddComponents(ComponentPtrs... componentPtrs)  {
        (AddComponent(componentPtrs), ...);
    }

    template<typename T>
    void RemoveComponent() {
        for (auto it = _components.begin(); it != _components.end(); ++it) {
            if (dynamic_cast<T*>(*it)) {
                _components.erase(it);
                delete *it;
                return;
            }
        }
        // std::cout << "Error: Tried to remove Component that was not found." << std::endl;
    }

    template<typename T>
    T* GetComponent() const {
        for (auto c : _components) {
            T* t = dynamic_cast<T*>(c);
            if (t != nullptr)
            //assert(t != nullptr);
                return t;
        }
        // std::cout << "Error: Tried to access a Component that does not exist." << std::endl;
        return nullptr;
    }
    
    std::vector<Component*> GetComponents() {
#ifdef SHOWDEBUG
        std::cout << "Getting components for entity " << this << std::endl;
        std::cout << "Component count for entity " << this << ": " << _components.size() << std::endl;
#endif
        std::vector<Component*> components;
        components.reserve(_components.size());
        for (auto c : _components) {
            //assert(c != nullptr);
            components.push_back(c);
#ifdef SHOWDEBUG
            std::cout << "    Found component: " << typeid(*c).name() << std::endl;
#endif
        }
#ifdef SHOWDEBUG
        std::cout << std::endl;
#endif
        return components;
    }
};
