
#include "stdafx.h"

#include "gamemath.h"

#include "debugutils.h"
#include "viewport.h"
#include "sprite.h"
#include "camera.h"
#include "time.h"


// global

namespace g {
    Time time;
    ViewPort viewport; // screen (window)
    Camera camera; // world camera

    // reference to window created in main
    SDL_Window* window = nullptr;

    // global renderer (reference to renderer created in main)
    SDL_Renderer* renderer = nullptr; 

    glm::ivec2 window_size = glm::ivec2(0,0);
}


bool InitSDL2() {
    std::cout << "Hello, World!" << std::endl;
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        ShowError("Failed to initialize SDL.");
        return false;
    }
    if (TTF_Init() < 0) {
        ShowError("Failed to initialize SDL_TTF.");
        return false;
    }
    if (IMG_Init(IMG_INIT_PNG) < 0) {
        ShowError("Failed to initialize SDL_IMG.");
        return false;
    }

    SDL_Window* _window = SDL_CreateWindow(
        "Game window", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 800,
        SDL_WINDOW_RESIZABLE
    );
    if (!_window) {
        ShowError("Failed to create SDL_Window.");
        return false;
    }

    SDL_Renderer* _renderer = SDL_CreateRenderer(
        _window,
        -1, 
        SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED
    );
    if (!_renderer) {
        ShowError("Failed to create SDL_Renderer.");
        return false;
    }

    // set global vars
    g::window = _window;
    g::renderer = _renderer;

    SDL_GetWindowSize(g::window, &g::window_size.x, &g::window_size.y);
    std::cout << "Window Size: " << Vec2toString(g::window_size) << std::endl;
}

void DestroySDL2() {
    std::cout << "Cleanup SDL2..." << std::endl;
    SDL_DestroyWindow(g::window);
    SDL_DestroyRenderer(g::renderer);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

void CorrectViewPortSize() {
    SDL_GetWindowSize(g::window, &g::window_size.x, &g::window_size.y);
    if (g::window_size.x > g::window_size.y) {
        g::viewport.size = glm::ivec2(g::window_size.y, g::window_size.y);
        g::viewport.position.x = (g::window_size.x/2) - (g::viewport.size.x/2);
        g::viewport.position.y = 0;
    }
    else if (g::window_size.x <= g::window_size.y) {
        g::viewport.size = glm::ivec2(g::window_size.x, g::window_size.x);
        g::viewport.position.y = (g::window_size.y/2) - (g::viewport.size.y/2);
        g::viewport.position.x = 0;
    }
}


int main() {

    InitSDL2();

    // init viewport
    g::viewport.position = glm::vec2(150.0f,0.0f);
    g::viewport.size = glm::vec2(g::window_size.x, g::window_size.y);


    g::camera.position = glm::vec2(0,0); // camera position (in the world)
    g::camera.size = glm::vec2(g::window_size.x, g::window_size.y); // camera size
    g::camera.origin = glm::vec2(g::viewport.position.x + (g::window_size.x/2), g::viewport.position.y + (g::window_size.y/2));
    g::camera.zoom = 4.0f;

    
    CorrectViewPortSize();


    Sprite* playerSprite;
    Sprite* enemySprite;

    std::vector<Sprite*> sprites;


    sprites.push_back(
        new Sprite(
            CTFS(g::renderer, IMG_Load("res/img/grass.png")),
            (SDL_Rect){0,0,32,32},
            glm::vec2(0.0f,0.0f))
    );
    Sprite* grassSprite = sprites[sprites.size()-1];




    // todo: load texture without surface
    // CTFS = BAD!
    sprites.push_back(
        new Sprite(
            CTFS(g::renderer, IMG_Load("res/img/dinoman-blue.png")),
            (SDL_Rect){0,0,24,24},
            glm::vec2(12.0f,12.0f))
    );
    enemySprite = sprites[sprites.size()-1];

    sprites.push_back(
        new Sprite(
            CTFS(g::renderer, IMG_Load("res/img/dinoman-yellow.png")),
            (SDL_Rect){0,0,24,24},
            glm::vec2(12.0f,12.0f))
    );
    playerSprite = sprites[sprites.size()-1]; // create copy (nice)


    std::cout << "Number of sprites in vector: " << sprites.size() << std::endl;
    std::cout << "Player sprite index: " << FindSpriteIndex(sprites, playerSprite) << std::endl;


    float movespeed = 300.0f;

    MoveSprite(playerSprite, glm::vec2(200, 200));
    MoveSprite(enemySprite, glm::vec2(300, 300));
    MoveSprite(grassSprite, glm::vec2(0,0));


    SetCameraPosition(&g::camera, playerSprite->position);


     SDL_Texture* viewport_texture = SDL_CreateTexture(
            g::renderer, 
            SDL_PIXELFORMAT_RGBA8888, 
            SDL_TEXTUREACCESS_TARGET, 
            g::viewport.size.x, 
            g::viewport.size.y
        );

    bool run = true;
    while (run) {
        g::time.UpdateFirst(SDL_GetTicks());

        SDL_GetMouseState(&g::viewport.mouse_position.x, &g::viewport.mouse_position.y);
        g::viewport.mouse_position -= g::viewport.position;
        //std::cout << "Mouse Pos: " << Vec2toString(g::viewport.mouse_position) << std::endl;

        const Uint8 *keyboard_state = SDL_GetKeyboardState(NULL);

        if (keyboard_state[SDL_SCANCODE_DOWN])  MoveSprite(playerSprite, glm::vec2(0, -movespeed*g::time.DeltaTime()));
        if (keyboard_state[SDL_SCANCODE_UP])    MoveSprite(playerSprite, glm::vec2(0, movespeed*g::time.DeltaTime()));
        if (keyboard_state[SDL_SCANCODE_LEFT])  MoveSprite(playerSprite, glm::vec2(movespeed*g::time.DeltaTime(), 0));
        if (keyboard_state[SDL_SCANCODE_RIGHT]) MoveSprite(playerSprite, glm::vec2(-movespeed*g::time.DeltaTime(), 0));

        if (keyboard_state[SDL_SCANCODE_S]) MoveSprite(enemySprite, glm::vec2(0, -movespeed*g::time.DeltaTime()));
        if (keyboard_state[SDL_SCANCODE_W]) MoveSprite(enemySprite, glm::vec2(0, movespeed*g::time.DeltaTime()));
        if (keyboard_state[SDL_SCANCODE_A]) MoveSprite(enemySprite, glm::vec2(movespeed*g::time.DeltaTime(), 0));
        if (keyboard_state[SDL_SCANCODE_D]) MoveSprite(enemySprite, glm::vec2(-movespeed*g::time.DeltaTime(), 0));

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_WINDOWEVENT) {
                if (event.window.event == SDL_WINDOWEVENT_CLOSE) run = false;

                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    CorrectViewPortSize();
                }
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) run = false;
            }

        }





        // center of camera

        glm::vec2 pos = WorldToScreen(&playerSprite->position, &g::viewport, &g::camera);
        //SetCameraPosition(&g::camera, playerSprite->position);
        SlowlyMoveCamera(&g::camera, playerSprite->position, 6.0f*g::time.DeltaTime());
        // SetCameraPosition(&g::camera, playerSprite->position);
        // std::cout << "enemySprite->position:  " << Vec2toString(pos) << std::endl;

        //std::cout << "playerSprite->position: " << Vec2toString(pos) << std::endl;



   
        SDL_SetRenderTarget(g::renderer, viewport_texture);

        // clear viewport_texture
        SDL_SetRenderDrawColor(g::renderer, 100, 100, 100, 255);
        SDL_RenderClear(g::renderer);

        for (auto &spr : sprites) {

            glm::vec2 on_screen_position = WorldToScreen(spr, &g::viewport, &g::camera);

            glm::vec2 on_screen_size = glm::vec2(0.0f, 0.0f);
            on_screen_size += glm::vec2(spr->src_rect.w, spr->src_rect.h);
            on_screen_size *= g::camera.zoom;


            SDL_Rect on_screen_rect = (SDL_Rect) {
                on_screen_position.x, on_screen_position.y,
                on_screen_size.x, on_screen_size.y
            };

            //SDL_RenderCopy(g::renderer, viewport_texture, &spr->src_rect, &on_screen_rect);
            SDL_RenderCopy(g::renderer, spr->texture, &spr->src_rect, &on_screen_rect);

            // debug lines -------------------------------------------
            /*SDL_SetRenderDrawColor(g::renderer, 255, 0, 0, 255);
            SDL_RenderDrawRect(g::renderer, &on_screen_rect);
            int a = 10;
            SDL_Rect spr_origin_rect = (SDL_Rect) {
                on_screen_rect.x + (spr->origin.x*g::camera.zoom) - (a/2),
                on_screen_rect.y + (spr->origin.y*g::camera.zoom) - (a/2),
                a,a
            };
            SDL_SetRenderDrawColor(g::renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(g::renderer, &spr_origin_rect);*/
        }

        SDL_SetRenderTarget(g::renderer, NULL);

        SDL_Rect viewport_rect = (SDL_Rect){
            g::viewport.position.x, 
            g::viewport.position.y, 
            g::viewport.size.x, 
            g::viewport.size.y
        };

        SDL_RenderCopy(g::renderer, viewport_texture, NULL, &viewport_rect);

        SDL_RenderPresent(g::renderer);

        SDL_SetRenderDrawColor(g::renderer, 0, 0, 0, 255);
        SDL_RenderClear(g::renderer);

        std::string debug_title = std::to_string(g::time.DeltaTime()) + ", " + std::to_string(g::time.FPS());
        SDL_SetWindowTitle(g::window, debug_title.c_str());

        g::time.UpdateLast(SDL_GetTicks());
    }
    std::cout << std::endl << "Main loop ended." << std::endl;


    DestroyAllSprites(&sprites);

    DestroySDL2();

    return 0;
}