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

    //SDL_SetHint("SDL_RENDER_BATCHING", "1");

    _window = SDL_CreateWindow("Game Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_RESIZABLE);
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
        glm::vec2(100.0f, 0.0f),
        GetWindowSize(),
        glm::vec2(viewPort.GetPosition() + glm::vec2(GetWindowSize()/2)),
        5.0f, 5.0f
    );

    SetRunning(true);
}

/*
    Game::~Game() destructor destroys all SDL subsystems, game systems and the entity manager. The entity manager will
    then cleanup all entities and their components.
*/
Game::~Game() {
    std::cout << "Destroying up game objects..." << std::endl;
    delete spriteSystem;
    delete animatorSystem;
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
                    UpdateWindowSize();
                    viewPort = GameViewPort(glm::ivec2(GetWindowSize()), glm::vec2(0.0f, 0.0f));
                    //CorrectViewPortSize();
                    break;
            }
        case SDL_KEYDOWN:
            switch (ev.key.keysym.sym) {
            case SDLK_ESCAPE:
                SetRunning(false);
                break;
            case SDLK_MINUS:
                camera.AddZoom(-1.0f);
                break;
            case SDLK_EQUALS:
                camera.AddZoom(1.0f);
                break;
            default:
                break;
            }
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

    if (GetKeyboardState()[SDL_SCANCODE_E]) std::cout << "Holding E." << std::endl;

    const float ms = 150.0f;

    glm::vec2 cam_movement = glm::vec2(
        (GetKeyboardState()[SDL_SCANCODE_RIGHT] * (-ms*time.DeltaTime())) -
        (GetKeyboardState()[SDL_SCANCODE_LEFT]  * (-ms*time.DeltaTime())),

        (GetKeyboardState()[SDL_SCANCODE_DOWN]  * (-ms*time.DeltaTime())) -
        (GetKeyboardState()[SDL_SCANCODE_UP]    * (-ms*time.DeltaTime()))
    );
    camera.Translate(cam_movement);
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
    spriteSystem = new SpriteSystem(_renderer, entityManager, _tileSet, &camera);
    animatorSystem = new AnimatorSystem(entityManager);

    playerSystem = new PlayerSystem(entityManager, &camera);

    
    // init entities?


    // create player
    entityManager->CreateEntity(
        new PlayerComponent(),
        new TransformComponent(glm::vec2(0.0f, 0.0f)),
        new SpriteComponent(SDL_Rect{ 432, 80, 16, 16 }),
        new AnimatorComponent(new Animation(SDL_Rect{432, 80, 16, 16}, 0.09f, 4)),
        new HealthComponent(100)
    );

    entityManager->CreateEntity(
        new TransformComponent(glm::vec2(100.0f, 100.0f)),
        new SpriteComponent(SDL_Rect{ 432, 32, 16, 16 }),
        new AnimatorComponent(new Animation(SDL_Rect{432, 32, 16, 16}, 0.03f, 4)),
        new HealthComponent(100)
    );


}

/*
    Game::Update() updates each game system each tick.
*/
void Game::Update() {
    // update game logic (run systems)
    playerSystem->Update(time.DeltaTime(), _keyboardState);
    animatorSystem->Update(time.DeltaTime());
}

/*
    Game::Render() updates each rendering system each frame. (delta time should not be required here.)
*/
void Game::Render() {
    // iterate through entities with sprite component
    spriteSystem->Render();

    // draw the mouse cursor
    SDL_SetRenderDrawColor(_renderer, 0, 255, 255, 255);
    SDL_Rect mouse_rect = SDL_Rect {viewPort.GetMousePosition().x - 2, viewPort.GetMousePosition().y - 2, 4, 4};
    SDL_RenderFillRect(_renderer, &mouse_rect);
}

/*
    Game::Run() runs the main game loop. Before the loop starts, it runs Game::Init().
*/
void Game::Run() {

    Init();

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
        Update();

        // clear the renderer before copying
        SDL_SetRenderDrawColor(_renderer, 100, 100, 100, 255);
        SDL_RenderClear(_renderer);
        // copy all textures to the buffer
        Render();
        // present the buffer
        SDL_RenderPresent(_renderer);

        // ------------------------- //
        time.UpdateLast(SDL_GetTicks()); 

        // print stats debug
        {
            static Uint64 cur;
            const int delay = 20;
            if (cur >= delay) {
                std::cout << "Tick: " << SDL_GetTicks64() << ", DT: " << time.DeltaTime() << ", FPS: " << time.FPS() << std::endl;
                cur = 0;
            }
            else cur++;
        }
    }
}