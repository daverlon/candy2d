#pragma once

#include "../stdafx.h"

#include "../GameClasses/EntityManager.h"

class Scene {
private:
    // data ?

    // EntityManager *_entityManager;

public:

    // notes? :
    // scenes should be on a stack
    // scenes should only initialize data,
    // the game engine should take that data and run it
    // the scene should not run any data, just initialize data for the 
    // systems in the core engine to run

    // to make customized functionality for each scene,
    // create a system that manages the gameplay,
    // specialized for each scene, etc

    Scene() {
        std::cout << "Scene() " << this << std::endl;
    }

    virtual ~Scene() {
        std::cout << "~Scene() " << this << std::endl;
    }


    virtual void Init() {

    } // initialize scene data
};