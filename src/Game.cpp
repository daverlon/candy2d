#include "Game.h"

/*
    Game::Game() constructor initializes SDL2, creates an SDL_Window and SDL_Renderer, 
    then sets up the game viewport and camera.
*/
Game::Game() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        ShowError("Failed to initialize SDL.");
        return;
    }

    if (TTF_Init() < 0) {
        ShowError("Failed to initialize SDL_TTF.");
        return;
    }

    if (IMG_Init(IMG_INIT_PNG) < 0) {
        ShowError("Failed to initialize SDL_IMG.");
        return;
    }

    // SDL_SetHint("SDL_RENDER_BATCHING", "1");

    _window = SDL_CreateWindow("Game Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, SDL_WINDOW_RESIZABLE);
    if (!_window) {
        ShowError("Failed to create SDL_Window.");
        return;
    }

    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (!_renderer) {
        ShowError("Failed to create SDL_Renderer.");
        return;
    }

    UpdateWindowSize();
    std::cout << "Window Size: " << Vec2toString(GetWindowSize()) << std::endl;

    // setup the viewport
    viewPort = GameViewPort(
        glm::ivec2(GetWindowSize()), 
        glm::vec2(0.0f, 0.0f)
    );

    // setup the camera
    camera = GameCamera(
        glm::vec2(0.0f, 0.0f),
        GetWindowSize(),
        glm::vec2(viewPort.GetPosition() + glm::vec2(GetWindowSize()/2)),
        1.0f
    );

    SetRunning(true);
}

/*
    Game::~Game() destructor destroys all SDL subsystems, game systems and the entity manager. The entity manager will
    then cleanup all entities and their components.
*/
Game::~Game() {
    std::cout << "Destroying up game objects..." << std::endl;

    // delete tilemap;


    delete tilemapSystem;

    // delete systems
    delete spriteSystem;
    delete animatorSystem;
    delete enemyAISystem;
    delete playerSystem;
    delete colliderSystem;


    delete entityManager;


    std::cout << "Destroying SDL...";
    SDL_DestroyTexture(_tileSet);

    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    std::cout << " Finished." << std::endl;
}

/*
    Game::HandleEvents() handles all SDL window events and single key presses.
*/
void Game::HandleEvents() {
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        switch (ev.type) {
        case SDL_WINDOWEVENT:
            if (ev.window.event == SDL_WINDOWEVENT_CLOSE) {
                SetRunning(false);
                break;
            }
            // on mac it seems that every the cursor
            // leave sthe window (in windows mode)
            // it triggers one of these events!
            if (ev.window.event == SDL_WINDOWEVENT_RESIZED
                || ev.window.event == SDL_WINDOWEVENT_RESTORED 
                || ev.window.event == SDL_WINDOWEVENT_MAXIMIZED
                || ev.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    std::cout << "Window event detected." << std::endl;
                    UpdateWindowSize();
                    viewPort = GameViewPort(glm::ivec2(GetWindowSize()), glm::vec2(0.0f, 0.0f));
                    camera.SetSize(GetWindowSize());
                    break;
            }
        case SDL_MOUSEBUTTONDOWN:
            switch (ev.button.button) {
                case SDL_BUTTON_RIGHT:
                    std::cout << "Mouse Position: " << Vec2toString(viewPort.GetMousePosition())
                              << ", World Position:" << Vec2toString(camera.ScreenToWorld(viewPort.GetMousePosition())) << std::endl;
                    std::cout << "Camera Position: " << Vec2toString(camera.GetPosition()) << std::endl;
                    break;
                default:
                    break;
            }
            break;
        case SDL_KEYDOWN:
            switch (ev.key.keysym.sym) {
            case SDLK_SPACE:
                // TogglePause(); 
                break;
            case SDLK_ESCAPE:
                SetRunning(false);
                break;
            case SDLK_1:
                TogglePause();
                break;
            default:
                break;
            }
            break;
        case SDL_MOUSEWHEEL:
            if (ev.wheel.y > 0) {
                camera.AddZoom(0.5f);
            } else if (ev.wheel.y < 0) {
                camera.AddZoom(-0.5f);
            }
            std::cout << "Camera Zoom: " << camera.GetZoom() << std::endl;
            break;
        default:
            break;
        }
    }
}

/*
    Game::UpdateKeyboardState() updates the _keyboardState variable with SDL_GetKeyboardState.
    _keyboardState is expected to be passed into systems that require input control.
*/
void Game::UpdateKeyboardState() {
    _keyboardState = SDL_GetKeyboardState(NULL);
}

/*
    Game::Init() initializes the entity manager and game systems. It also loads the tile atlas. 
*/
void Game::Init() {
    // init tileset
    _tileSet = IMG_LoadTexture(_renderer, "resources/tileset.png");

    // init entity manager
    entityManager = new EntityManager();

    // init systems

    tilemapSystem = new TilemapSystem(_renderer, entityManager, _tileSet, &camera);

    spriteSystem = new SpriteSystem(_renderer, entityManager, _tileSet, &camera);
    animatorSystem = new AnimatorSystem(entityManager);
    playerSystem = new PlayerSystem(entityManager, &camera);
    colliderSystem = new ColliderSystem(entityManager, &camera);

    enemyAISystem = new EnemyAISystem(entityManager);
    skullBulletSystem = new SkullBulletSystem(entityManager);

    
    MainScene* mainScene = new MainScene(entityManager);
    mainScene->Init();
}

/*
    Game::Update() updates each game system each tick.
*/
void Game::Update() {
    // update game logic (run systems)
    entityManager->DeleteFlaggedEntities();

    playerSystem->Update(time.DeltaTime(), _keyboardState);
    animatorSystem->Update(time.DeltaTime());
    // enemyAISystem->Update(time.DeltaTime());
    skullBulletSystem->Update(time.DeltaTime());
    colliderSystem->Update(time.DeltaTime());
}

/*
    Game::Render() updates each rendering system each frame. (delta time should not be required here.)
*/
void Game::Render() {

    tilemapSystem->Render();

    spriteSystem->Render();
    colliderSystem->Render(_renderer);
    

    // draw the mouse cursor
    SDL_SetRenderDrawColor(_renderer, 0, 255, 255, 255);
    SDL_Rect mouse_rect = SDL_Rect {viewPort.GetMousePosition().x - 2, viewPort.GetMousePosition().y - 2, 4, 4};
    SDL_RenderFillRect(_renderer, &mouse_rect);

    // draw crosshair
    // SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 75);
    // const SDL_Point v_ps[2] = {
    //     (SDL_Point){GetWindowSize().x / 2, 0}, 
    //     (SDL_Point){GetWindowSize().x / 2, GetWindowSize().y},
    // };
    // const SDL_Point h_ps[2] = {
    //     (SDL_Point){0, GetWindowSize().y / 2},
    //     (SDL_Point){GetWindowSize().x, GetWindowSize().y / 2}
    // };
    // // std::cout << Vec2toString(GetWindowSize()) << std::endl;
    // SDL_RenderDrawLines(_renderer, v_ps, 2);
    // SDL_RenderDrawLines(_renderer, h_ps, 2);
}

/*
    Game::Run() runs the main game loop. Before the loop starts, it runs Game::Init().
*/
void Game::Run() {

    Init();
    _paused = true;

    // main loop
    while (GetRunning()) {
        time.UpdateFirst(SDL_GetTicks64()); 
        // ------------------------- //

        // update cursor position in the viewport
        viewPort.UpdateMousePosition();

        // process SDL event queue
        HandleEvents();

        // handle keyboard state (keys held)
        UpdateKeyboardState();

        // process game logic
        // std::cout << "Update" << std::endl;
        if (!_paused)
            Update();
        else {
            if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(2))
                camera.MoveSlowly(camera.ScreenToWorld(viewPort.GetMousePosition()), 0.015f);
        }

        // clear the renderer before copying
        SDL_SetRenderDrawColor(_renderer, 100, 100, 100, 255);
        SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
        SDL_RenderClear(_renderer);
        // copy all textures to the buffer
        Render();
        // present the buffer
        SDL_RenderPresent(_renderer);
  
        // ------------------------- //

        // print stats debug
        static Uint64 cur;
        const int delay = 60;
        if (cur >= delay) {
            std::cout << "Tick: " << SDL_GetTicks64() << ", DT: " << time.DeltaTime() << ", FPS: " << time.FPS() << ", ";
            std::cout << Vec2toString(viewPort.GetMousePosition())
            << ", " << Vec2toString(camera.ScreenToWorld(viewPort.GetMousePosition())) << std::endl;
            std::cout << "SDL_Error: " << SDL_GetError() << std::endl;
            cur = 0;
        }
        else cur++;

        time.UpdateLast(SDL_GetTicks()); 
    }
}
