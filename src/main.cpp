#include <iostream>
#include <string>
#include <vector>
#include <format>
#include <algorithm>


#include "sprite.h"
#include "camera.h"



#include "time.h"

_Time Time;


void ShowError(std::string msg) {
    std::cout << msg << std::endl;
    std::cout << SDL_GetError() << std::endl;
}





int main() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)  ShowError("Failed to initialize SDL.");
    if (TTF_Init() < 0)                     ShowError("Failed to initialize SDL_TTF.");
    if (IMG_Init(IMG_INIT_PNG) < 0)         ShowError("Failed to initialize SDL_IMG.");

    SDL_Window* window = SDL_CreateWindow(
        "Game window", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 800,
        SDL_WINDOW_RESIZABLE
    );
    if (!window) ShowError("Failed to create SDL_Window.");

 
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, 
        -1, 
        SDL_RENDERER_PRESENTVSYNC
    );
    if (!renderer) ShowError("Failed to create SDL_Renderer.");

    int window_width = 0;
    int window_height = 0;
    SDL_GetWindowSize(window, &window_width, &window_height);


    Camera* cam = new Camera((SDL_Rect){50, 50, window_width, window_height}, 4);

    Sprite* playerSprite;
    Sprite* enemySprite;

    std::vector<Sprite*> sprites;

    sprites.push_back(
        new Sprite(
            CTFS(renderer, IMG_Load("res/img/dinoman-blue.png")),
            (SDL_Rect){0,0,24,24},
            12,12)
    );
    enemySprite = sprites[sprites.size()-1];

    sprites.push_back(
        new Sprite(
            CTFS(renderer, IMG_Load("res/img/dinoman-yellow.png")),
            (SDL_Rect){0,0,24,24},
            12,12)
    );
    playerSprite = sprites[sprites.size()-1]; // create copy (nice)

    SetCameraFocus(cam, playerSprite);


    std::cout << "Number of sprites in vector: " << sprites.size() << std::endl;
    std::cout << "Player sprite index: " << FindSpriteIndex(sprites, playerSprite) << std::endl;


    float movespeed = 300;

    MoveSprite(playerSprite, 200, 200);
    MoveSprite(enemySprite, 300, 300);


    bool run = true;
    while (run) {
        Time.UpdateFirst(SDL_GetTicks());

        const Uint8 *keyboard_state = SDL_GetKeyboardState(NULL);

        if (keyboard_state[SDL_SCANCODE_DOWN])  MoveSprite(playerSprite, 0, movespeed*Time.DeltaTime());
        if (keyboard_state[SDL_SCANCODE_UP])    MoveSprite(playerSprite, 0, -movespeed*Time.DeltaTime());
        if (keyboard_state[SDL_SCANCODE_LEFT])  MoveSprite(playerSprite, -movespeed*Time.DeltaTime(), 0);
        if (keyboard_state[SDL_SCANCODE_RIGHT]) MoveSprite(playerSprite, movespeed*Time.DeltaTime(), 0);


        if (keyboard_state[SDL_SCANCODE_S]) MoveSprite(enemySprite, 0, movespeed*Time.DeltaTime());
        if (keyboard_state[SDL_SCANCODE_W]) MoveSprite(enemySprite, 0, -movespeed*Time.DeltaTime());
        if (keyboard_state[SDL_SCANCODE_A]) MoveSprite(enemySprite, -movespeed*Time.DeltaTime(), 0);
        if (keyboard_state[SDL_SCANCODE_D]) MoveSprite(enemySprite, movespeed*Time.DeltaTime(), 0);


        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_WINDOWEVENT) {
                if (event.window.event == SDL_WINDOWEVENT_CLOSE) run = false;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) run = false;
            }
        }

        SetCameraPosition(cam, 
             cam->focus->img_pos.x - (cam->focus->img_rect.w/2), 
             cam->focus->img_pos.y - (cam->focus->img_rect.h/2));

        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderClear(renderer);

        for (int i = 0; i < sprites.size(); i++) {
            const Sprite *spr = sprites[i];

            SDL_Rect spr_rect_cam_view = (SDL_Rect){
                cam->view.x + (cam->view.w/2) - spr->img_pos.x - ((spr->origin_x)*cam->zoom),
                cam->view.y + (cam->view.h/2) - spr->img_pos.y - (spr->origin_x*cam->zoom),
                spr->img_pos.w * cam->zoom, 
                spr->img_pos.h * cam->zoom
            };
            SDL_RenderCopy(renderer, spr->texture, &spr->img_rect, &spr_rect_cam_view);
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        }

        SDL_RenderPresent(renderer);

        Time.UpdateLast(SDL_GetTicks());

        std::string debug_title = std::to_string(Time.DeltaTime()) + ", " + std::to_string(Time.FPS());
        SDL_SetWindowTitle(window, debug_title.c_str());
    }
    std::cout << std::endl << "Main loop ended." << std::endl;

    DestroyCamera(cam);
    DestroyAllSprites(&sprites);

    std::cout << "Cleanup SDL2..." << std::endl;
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    return 0;
}