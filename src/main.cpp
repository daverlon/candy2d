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

void PrintVec2(glm::vec2 v) {
    std::cout << "(" << v.x << "," << v.y << ")" << std::endl;
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


    Camera* cam = new Camera(glm::vec2(0,0), glm::vec2(window_width, window_height), 4);

    Sprite* playerSprite;
    Sprite* enemySprite;

    std::vector<Sprite*> sprites;




    sprites.push_back(
        new Sprite(
            CTFS(renderer, IMG_Load("res/img/grass.png")),
            (SDL_Rect){0,0,32,32},
            glm::vec2(0.0f,0.0f))
    );
    Sprite* grassSprite = sprites[sprites.size()-1];





    sprites.push_back(
        new Sprite(
            CTFS(renderer, IMG_Load("res/img/dinoman-blue.png")),
            (SDL_Rect){0,0,24,24},
            glm::vec2(12.0f,12.0f))
    );
    enemySprite = sprites[sprites.size()-1];

    sprites.push_back(
        new Sprite(
            CTFS(renderer, IMG_Load("res/img/dinoman-yellow.png")),
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

    //SDL_Texture* tx_target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 800, 800);

    SetCameraPosition(cam, playerSprite->position);

    int margin = 250;
    float camspeed = 5.0f;

    bool run = true;
    while (run) {
        Time.UpdateFirst(SDL_GetTicks());

        const Uint8 *keyboard_state = SDL_GetKeyboardState(NULL);

        if (keyboard_state[SDL_SCANCODE_DOWN])  MoveSprite(playerSprite, glm::vec2(0, -movespeed*Time.DeltaTime()));
        if (keyboard_state[SDL_SCANCODE_UP])    MoveSprite(playerSprite, glm::vec2(0, movespeed*Time.DeltaTime()));
        if (keyboard_state[SDL_SCANCODE_LEFT])  MoveSprite(playerSprite, glm::vec2(movespeed*Time.DeltaTime(), 0));
        if (keyboard_state[SDL_SCANCODE_RIGHT]) MoveSprite(playerSprite, glm::vec2(-movespeed*Time.DeltaTime(), 0));


        if (keyboard_state[SDL_SCANCODE_S]) MoveSprite(enemySprite, glm::vec2(0, -movespeed*Time.DeltaTime()));
        if (keyboard_state[SDL_SCANCODE_W]) MoveSprite(enemySprite, glm::vec2(0, movespeed*Time.DeltaTime()));
        if (keyboard_state[SDL_SCANCODE_A]) MoveSprite(enemySprite, glm::vec2(movespeed*Time.DeltaTime(), 0));
        if (keyboard_state[SDL_SCANCODE_D]) MoveSprite(enemySprite, glm::vec2(-movespeed*Time.DeltaTime(), 0));


        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_WINDOWEVENT) {
                if (event.window.event == SDL_WINDOWEVENT_CLOSE) run = false;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) run = false;
            }
        }


        // center of camera
        glm::vec2 cam_origin = cam->position + (cam->view_size/2.0f);



        SDL_Rect player_screen_rect = (SDL_Rect){
            cam_origin.x - playerSprite->position.x + playerSprite->origin.x*cam->zoom,
            cam_origin.x - playerSprite->position.y + playerSprite->origin.y*cam->zoom,
            playerSprite->src_rect.w*cam->zoom,
            playerSprite->src_rect.h*cam->zoom
        };


        if (player_screen_rect.x < margin) MoveCamera(cam, 5.0f, 0.0f);
        if (player_screen_rect.x-player_screen_rect.w > cam->view_size.x-margin) MoveCamera(cam, -5.0f, 0.0f);

        if (player_screen_rect.y < margin) MoveCamera(cam, 0.0f, 5.0f);
        //if (player_screen_rect.y-player_screen_rect.h > cam->view_size.y-margin) MoveCamera(cam, 0.0f, 5.0f);


        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderClear(renderer);




        std::cout << " enemySprite->position: (" << enemySprite->position.x << "," << enemySprite->position.y << ")" << std::endl;
        std::cout << "playerSprite->position: (" << playerSprite->position.x << "," << playerSprite->position.y << ")" << std::endl;




        for (auto &spr : sprites) {

            glm::vec2 adjPos;
            adjPos = spr->position + spr->origin*cam->zoom;

            SDL_Rect sprite_screen_rect = (SDL_Rect){
                cam_origin.x - adjPos.x,
                cam_origin.y - adjPos.y,
                spr->src_rect.w*cam->zoom,
                spr->src_rect.h*cam->zoom
            };

            SDL_RenderCopy(renderer, spr->texture, &spr->src_rect, &sprite_screen_rect);



            // debug lines

            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &sprite_screen_rect);

            int a = 10;
            SDL_Rect spr_origin_rect = (SDL_Rect) {
                sprite_screen_rect.x + (spr->origin.x*cam->zoom) - (a/2),
                sprite_screen_rect.y + (spr->origin.y*cam->zoom) - (a/2),
                a,a
            };
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(renderer, &spr_origin_rect);


        }


        //SDL_SetRenderTarget(renderer, NULL);
        // SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        // SDL_RenderClear(renderer);


        //SDL_RenderCopy(renderer, tx_target, NULL, NULL);

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