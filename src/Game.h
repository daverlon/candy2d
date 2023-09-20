#pragma once

#include "stdafx.h"
#include "Utils/DebugUtils.h"

#include "GameClasses/GameTime.h"
#include "GameClasses/GameCamera.h"
#include "GameClasses/GameViewPort.h"

#include "GameClasses/EntityManager.h"

// components
#include "Components/TransformComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/AnimatorComponent.h"

#include "Components/TilemapComponent.h"

// systems
#include "Systems/SpriteSystem.h"
#include "Systems/AnimatorSystem.h"
#include "Systems/PlayerSystem.h"
#include "Systems/EnemyAISystem.h"
#include "Systems/SkullBulletSystem.h"
#include "Systems/ColliderSystem.h"

#include "Systems/TilemapSystem.h"

// scenes
#include "Scenes/MainScene.h"

class Game {
private:
    SDL_Window* _window = nullptr;
    glm::ivec2 _windowSize = glm::ivec2(0, 0);

    SDL_Renderer* _renderer = nullptr;

    bool _running = false;
    bool _paused = false;

    SDL_Texture* _tileSet = nullptr;

    const Uint8* _keyboardState; // hold keyboard state

public:
    Game();
    ~Game();

    GameTime time;
    GameCamera camera;
    GameViewPort viewPort;

    // entity manager
    EntityManager* entityManager;
    

    // systems
    SpriteSystem* spriteSystem;
    AnimatorSystem* animatorSystem;

    PlayerSystem* playerSystem;
    EnemyAISystem* enemyAISystem;
    SkullBulletSystem* skullBulletSystem;

    ColliderSystem* colliderSystem;

    TilemapSystem* tilemapSystem;


    void HandleEvents();

    void UpdateKeyboardState();
    const Uint8* GetKeyboardState() const { return _keyboardState; }

    void Init();
    void Update();
    void Render();
    void Run();

    inline bool GetRunning() { return _running; } 
    inline void SetRunning(bool b) { _running = b; }
    inline void TogglePause() { _paused = !_paused; }

    inline void UpdateWindowSize() { 
        glm::ivec2 s = GetWindowSize();
        SDL_GetWindowSize(_window, &_windowSize.x, &_windowSize.y ); 
        std::cout << Vec2toString(s) << " Window size updated to: " << Vec2toString(_windowSize) << std::endl;
    }
    inline glm::ivec2 GetWindowSize() { return _windowSize; }


};
