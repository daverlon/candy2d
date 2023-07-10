#pragma once

#include "stdafx.h"
#include "Utils/DebugUtils.h"

#include "GameClasses/GameTime.h"
#include "GameClasses/GameCamera.h"
#include "GameClasses/GameViewPort.h"

#include "GameClasses/EntityManager.h"

#include "Components/TransformComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/AnimatorComponent.h"

#include "Systems/SpriteSystem.h"
#include "Systems/AnimatorSystem.h"
#include "Systems/PlayerSystem.h"
#include "Systems/EnemyAISystem.h"
#include "Systems/ColliderSystem.h"

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

    ColliderSystem* colliderSystem;


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

    inline void UpdateWindowSize() { SDL_GetWindowSize(_window, &_windowSize.x, &_windowSize.y ); }
    inline glm::ivec2 GetWindowSize() { return _windowSize; }
};