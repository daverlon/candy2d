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
        5.0f, 15.0f
    );

    SetRunning(true);
}

/*
    Game::~Game() destructor destroys all SDL subsystems, game systems and the entity manager. The entity manager will
    then cleanup all entities and their components.
*/
Game::~Game() {
    std::cout << "Destroying up game objects..." << std::endl;

    delete tilemap;

    delete spriteSystem;
    delete animatorSystem;
    delete enemyAISystem;
    delete playerSystem;
    // delete colliderSystem;


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
            if (ev.button.button == SDL_BUTTON_LEFT) {
                std::cout << "Mouse Position: " << Vec2toString(viewPort.GetMousePosition())
                << ", World Position:" << Vec2toString(camera.ScreenToWorld(viewPort.GetMousePosition())) << std::endl;

                std::cout << "Camera Position: " << Vec2toString(camera.GetPosition()) << std::endl;
            }
            break;
        case SDL_KEYDOWN:
            switch (ev.key.keysym.sym) {
            case SDLK_SPACE:
                TogglePause(); 
                break;
            case SDLK_ESCAPE:
                SetRunning(false);
                break;
            default:
                break;
            }
            break;
        case SDL_MOUSEWHEEL:
            if (ev.wheel.y > 0) {
                camera.AddZoom(1.0f);
                std::cout << "Camera:" << camera.GetZoom() << "/" << camera.GetOriginalZoom() << std::endl;
            } else if (ev.wheel.y < 0) {
                camera.AddZoom(-1.0f);
                std::cout << "Camera:" << camera.GetZoom() << "/" << camera.GetOriginalZoom() << std::endl;
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
    enemyAISystem = new EnemyAISystem(entityManager);
    colliderSystem = new ColliderSystem(entityManager, &camera);

    
    // init entities?

    // create player
    // wizard_m_run 192 164 16 28 4
    entityManager->CreateEntity(
        new PlayerComponent(),
        new TransformComponent(glm::vec2(-384.0f, -190.0f), glm::vec2(8.0f, 24.0f)),
        new SpriteComponent(SDL_Rect{ 192, 164, 16, 28 }),
        new AnimatorComponent(new Animation(SDL_Rect{192, 164, 16, 28}, 0.09f, 4)),
        // new HealthComponent(100),
        new ColliderComponent(glm::vec4(3.0f, 14.0f, 10.0f, 13.0f), false, false)
    );

    // create enemies
    for (int i = 0; i < 30; i++)
        entityManager->CreateEntity(
            new TransformComponent(glm::vec2(-300.0f + 25.0f * -i, -100.0f)),
            new SpriteComponent(SDL_Rect{ 368, 369, 16, 23 }),
            new AnimatorComponent(new Animation(SDL_Rect{368, 369, 16, 23}, 0.03f, 4)),
            // new HealthComponent(100),
            new EnemyAIComponent(),
            new ColliderComponent(glm::vec4(3.0f, 12.0f, 10.0f, 10.0f), false, false)
        );

    // crate 288 298 16 22
    entityManager->CreateEntity(
        new TransformComponent(glm::vec2(-400.0f, -150.0f), glm::vec2(8.0f, 19.0f)),
        new SpriteComponent(SDL_Rect{288, 408, 16, 24}),
        new ColliderComponent(glm::vec4(1.0f, 5.0f, 14.0f, 17.0f), false, false)
    );

    // chest_empty_open_anim 304 288 16 16 3
    entityManager->CreateEntity(
        new TransformComponent(glm::vec2(-350.0f, -150.0f), glm::vec2(8.0f, 13.0f)),
        new SpriteComponent(SDL_Rect{304, 400, 16, 16}),
        new AnimatorComponent(new Animation(SDL_Rect{304, 400, 16, 16}, 0.1f, 3)),
        new ColliderComponent(glm::vec4(1.0f, 5.0f, 14.0f, 11.0f), false, true)
    );

    // add idle ogre
    entityManager->CreateEntity(
        new TransformComponent(glm::vec2(-330.0f, -200.0f), glm::vec2(16.0f, 29.0f)),
        new SpriteComponent(SDL_Rect{16, 380, 32, 36}, SPRITE_LAYER_YSORT),
        new AnimatorComponent(new Animation(SDL_Rect{16, 380, 32, 36}, 0.2f, 4)),
        new ColliderComponent(glm::vec4(6.0f, 16.f, 20.0f, 14.0f), false, true)
    );


    // map colliders
    entityManager->CreateEntity(
        new TransformComponent(glm::vec2(0.0f, 0.0f)),
        new ColliderComponent(glm::vec4(0.0f, 0.0f, 770, 70), false, true)
    );

    entityManager->CreateEntity(
        new TransformComponent(glm::vec2(0.0f, -365.0f)),
        new ColliderComponent(glm::vec4(0.0f, 0.0f, 770, 32), false, true)
    );

    entityManager->CreateEntity(
        new TransformComponent(glm::vec2(19.0f, 0.0f)),
        new ColliderComponent(glm::vec4(0.0f, 0.0f, 39, 365), false, true)
    );
    
    entityManager->CreateEntity(
        new TransformComponent(glm::vec2(-748.0f, 0.0f)),
        new ColliderComponent(glm::vec4(0.0f, 0.0f, 32, 365), false, true)
    );

    std::vector<SDL_Rect> tileRects = {
        // floor tiles (0 - 8)
        {16, 64, 16, 16},
        {32, 64, 16, 16},
        {48, 64, 16, 16},
        {16, 80, 16, 16},
        {32, 80, 16, 16},
        {48, 80, 16, 16},
        {16, 96, 16, 16},
        {32, 96, 16, 16},

        // top wall tiles

        {32, 0, 16, 16},   // wall very top (index 8)

        {32, 16, 16, 16}, // top wall (index 9)
        {16, 16, 16, 16}, // top wall (index 10)
        {48, 32, 16, 16}, // top wall hole 1 (index 11)
        {48, 48, 16, 16}, // top wall hole 2 (index 12)
        {31, 120, 16, 16}, // top left corner (13)
        {0, 152, 16, 16}, // wall left (14)
        {0, 136, 16, 16}, // wall bottom (15)
        {32, 168, 16, 16}, // bottom left corner (16)
        {0, 136, 16, 16}, // top left corner (17)
        {16, 152, 16, 16}, // right wall (18)
    };

    int cleans[3] = {3, 5, 7};
    int dirtys[4] = {0, 1, 2, 4};
    const int gridWidth = 48;
    const int gridHeight = 24;

    std::vector<std::vector<int>> data(gridHeight, std::vector<int>(gridWidth));

    for (int y = 0; y < gridHeight; y++) {
        for (int x = 0; x < gridWidth; x++) {

            int id = 0;
            // top wall
            // very top wall edge
            if (y == 0) {
                if (x == 0) {
                    // top left corner
                    id = 14; // wall top
                    data[y][x] = id;
                    continue;
                }
                

                data[y][x] = 8; // wall top
                continue;
            }

            // wall left
            if (x == 0) {
                id = 14;
                data[y][x] = id;
                continue;
            }

            if (y == gridHeight - 1) {
                id = 9;
                data[y][x] = id; // wall top
                continue;
            }
            if (y == gridHeight - 1) {
                if (x == 0) {
                    // bottom left corner 
                    id = 16;
                    data[y][x] = id; 
                    continue;
                }

                id = 8;
                data[y][x] = id; // wall top
                continue;
            }

            // right wall
            if (x == gridWidth - 1)  {
                id = 18;
                data[y][x] = id;
                continue;
            }

            if (y >= 1 && y <= 4) {
                int r = rand() % 100;
                if (r < 60) {
                   id = 9; // top wall
                }
                else if (r >= 60 && r < 70) {
                    id = 12; // top wall
                }
                else if (r >= 70 && r <= 85) {
                    id = 10; // top wall
                }
                else {
                    id = 11; // top wall
                }
                data[y][x] = id;
                continue;
            }

            int idx = rand() % 100;
            int idx2 = 0;

            if (idx < 75) {
                idx2 = cleans[rand() % 3]; // 75% chance to draw clean tile
            }
            else if (idx >= 75 && idx <= 95) {
                idx2 = 6; // 15% chance of drawing medium clean tile
            }
            else {
                idx2 = dirtys[rand() % 4]; // 10% chance of drawing dirty tile
            }

            data[y][x] = 7 - idx2;
        }
    }

    tilemap = new Tilemap(_renderer, &camera, _tileSet, glm::ivec2(0, 0), glm::ivec2(16, 16));
    tilemap->GenerateTexture(data, tileRects);
}

/*
    Game::Update() updates each game system each tick.
*/
void Game::Update() {
    // update game logic (run systems)
    playerSystem->Update(time.DeltaTime(), _keyboardState);
    animatorSystem->Update(time.DeltaTime());
    enemyAISystem->Update(time.DeltaTime());
    colliderSystem->Update();
}

/*
    Game::Render() updates each rendering system each frame. (delta time should not be required here.)
*/
void Game::Render() { // iterate through entities with sprite component

    tilemap->Render();

    spriteSystem->Render();
    colliderSystem->Render(_renderer);
    

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
        if (!_paused)
            Update();

        // clear the renderer before copying
        SDL_SetRenderDrawColor(_renderer, 100, 100, 100, 255);
        // SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
        SDL_RenderClear(_renderer);
        // copy all textures to the buffer
        Render();
        // present the buffer
        SDL_RenderPresent(_renderer);

        // ------------------------- //
        time.UpdateLast(SDL_GetTicks()); 

        // print stats debug
        if (!_paused)
        {
            static Uint64 cur;
            const int delay = 60;
            if (cur >= delay) {
                std::cout << "Tick: " << SDL_GetTicks64() << ", DT: " << time.DeltaTime() << ", FPS: " << time.FPS() << ", ";
                std::cout << Vec2toString(viewPort.GetMousePosition())
                << ", " << Vec2toString(camera.ScreenToWorld(viewPort.GetMousePosition())) << std::endl;
                cur = 0;
            }
            else cur++;
        }
    }
}
