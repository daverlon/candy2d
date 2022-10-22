#include "game.h"

#include "stdafx.h"
#include "gamemath.h"
#include "debugutils.h"
#include "viewport.h"
#include "sprite.h"
#include "camera.h"
#include "time.h"
#include "tilemap.h"

namespace g {
    Time time;

    ViewPort viewport; // screen (window)
    Camera camera; // world camera

    SDL_Window* window = nullptr;
    glm::ivec2 window_size = glm::ivec2(0,0);

    SDL_Renderer* renderer = nullptr; 
}

void InitViewPort() {
    g::viewport.position = glm::vec2(150.0f,0.0f);
    g::viewport.size = glm::vec2(g::window_size.x, g::window_size.y);
}

void InitCamera() {
    g::camera.position = glm::vec2(0,0); // camera position (in the world)
    g::camera.size = glm::vec2(g::window_size.x, g::window_size.y); // camera size
    g::camera.origin = glm::vec2(g::viewport.position.x + (g::window_size.x/2), g::viewport.position.y + (g::window_size.y/2));
    g::camera.orig_zoom = 5.0f;
    g::camera.zoom = 4.0f;
}

void RunGame() {

    InitViewPort();
    InitCamera();
    CorrectViewPortSize();

    Sprite* playerSprite;
    Sprite* enemySprite;

    std::vector<Sprite*> sprites;

    sprites.push_back(
        new Sprite(
            IMG_LoadTexture(g::renderer, "res/img/grass.png"),
            (SDL_Rect){0,0,32,32},
            glm::vec2(0.0f,0.0f))
    );
    Sprite* grassSprite = sprites[sprites.size()-1];

    sprites.push_back(
        new Sprite(
            IMG_LoadTexture(g::renderer, "res/img/dinoman-blue.png"),
            (SDL_Rect){0,0,24,24},
            glm::vec2(12.0f,12.0f))
    );
    enemySprite = sprites[sprites.size()-1];

    sprites.push_back(
        new Sprite(
            IMG_LoadTexture(g::renderer, "res/img/dinoman-yellow.png"),
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

    TileMap my_tilemap = TileMap(glm::ivec2(64,64));
    LoadTileMap(
        &my_tilemap.texture, 
        "res/img/test_tilemap.png",
        "1,2,n,3,4,",
        "1,2,n,3,4,",
        glm::ivec2(32,32)
    );

    SDL_Texture* viewport_texture = nullptr;

    int target_index = 2;
    // todo: pause loop when window isn't focused
    bool run = true;
    while (run) {
        g::time.UpdateFirst(SDL_GetTicks());

        // reset viewport texture
        SDL_SetRenderTarget(g::renderer, viewport_texture);
        SDL_RenderClear(g::renderer);
        SDL_SetRenderTarget(g::renderer, NULL);
        viewport_texture = SDL_CreateTexture(
            g::renderer, 
            SDL_PIXELFORMAT_RGBA8888, 
            SDL_TEXTUREACCESS_TARGET, 
            g::viewport.size.x, 
            g::viewport.size.y
        );
        SDL_SetRenderTarget(g::renderer, NULL);

        UpdateMousePosition(g::viewport);

        const Uint8 *keyboard_state = SDL_GetKeyboardState(NULL);
        glm::vec2 player_movement = glm::vec2(
            (keyboard_state[SDL_SCANCODE_RIGHT] * (-movespeed*g::time.DeltaTime())) -
            (keyboard_state[SDL_SCANCODE_LEFT]  * (-movespeed*g::time.DeltaTime())),

            (keyboard_state[SDL_SCANCODE_DOWN]  * (-movespeed*g::time.DeltaTime())) -
            (keyboard_state[SDL_SCANCODE_UP]    * (-movespeed*g::time.DeltaTime()))
        );
        glm::normalize(player_movement);
        MoveSprite(playerSprite, player_movement);

        if (keyboard_state[SDL_SCANCODE_S]) MoveSprite(enemySprite, glm::vec2(0, -movespeed*g::time.DeltaTime()));
        if (keyboard_state[SDL_SCANCODE_W]) MoveSprite(enemySprite, glm::vec2(0, movespeed*g::time.DeltaTime()));
        if (keyboard_state[SDL_SCANCODE_A]) MoveSprite(enemySprite, glm::vec2(movespeed*g::time.DeltaTime(), 0));
        if (keyboard_state[SDL_SCANCODE_D]) MoveSprite(enemySprite, glm::vec2(-movespeed*g::time.DeltaTime(), 0));

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
                    run = false;
                    break;
                }
                // on mac it seems that every the cursor
                // leave sthe window (in windows mode)
                // it triggers one of these events!
                if (event.window.event == SDL_WINDOWEVENT_RESIZED
                    || SDL_WINDOWEVENT_RESTORED 
                    || SDL_WINDOWEVENT_MAXIMIZED
                    || SDL_WINDOWEVENT_SIZE_CHANGED) {
                        CorrectViewPortSize();
                        break;
                }
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    run = false;
                    break;
                }
                if (event.key.keysym.sym == SDLK_e) {
                    {
                        glm::ivec2 screenpos = g::viewport.mouse_position;
                        std::cout << "\n\n\nMouse screen pos: " << Vec2toString(screenpos) << std::endl;
                        glm::ivec2 worldpos = ScreenToWorld(screenpos, g::viewport, g::camera);
                        std::cout << "Mouse world  pos: " << Vec2toString(worldpos) << std::endl;
                    }
                    break;
                }
                if (event.key.keysym.sym == SDLK_f) {
                    if (target_index == 2) target_index = 1;
                    else if (target_index == 1) target_index = 2;
                    break;
                }
                if (event.key.keysym.sym == SDLK_SPACE) {
                    SetSpritePosition(grassSprite, ScreenToWorld(g::viewport.mouse_position, g::viewport, g::camera));
                    break;
                }
                if (event.key.keysym.sym == SDLK_EQUALS) {
                    g::camera.zoom += 0.1f;
                    std::cout << "Camera zoom: " << g::camera.zoom << "/" << g::camera.orig_zoom << std::endl;
                    break;
                }
                if (event.key.keysym.sym == SDLK_MINUS) {
                    g::camera.zoom -= 0.1f;
                    std::cout << "Camera zoom: " << g::camera.zoom << "/" << g::camera.orig_zoom << std::endl;
                    break;
                }
            }
        }

        SlowlyMoveCamera(&g::camera, sprites[target_index]->position, 6.0f*g::time.DeltaTime());

        // clear the window texture
        SDL_SetRenderTarget(g::renderer, NULL);
        SDL_SetRenderDrawColor(g::renderer, 50, 50, 50, 255);
        SDL_RenderClear(g::renderer);
   
        SDL_SetRenderTarget(g::renderer, viewport_texture);
        // clear viewport_texture
        SDL_SetRenderDrawColor(g::renderer, 100, 100, 100, 255);
        SDL_RenderClear(g::renderer);

        glm::ivec2 tilemap_pos = WorldToScreen(glm::ivec2(400, 400), g::viewport, g::camera);
        glm::ivec2 tilemap_size = glm::vec2(64,64) * g::camera.zoom;

        SDL_Rect tilemap_rect = Vec2Vec2toRect(tilemap_pos, tilemap_size);
        SDL_RenderCopy(g::renderer, &(*my_tilemap.texture), NULL, &tilemap_rect);

        // send sprites to viewport texture
        for (auto &spr : sprites) {

            glm::vec2 on_screen_position = WorldToScreen(*spr, g::viewport, g::camera);

            glm::vec2 on_screen_size = glm::vec2(0.0f, 0.0f);
            on_screen_size += glm::vec2(spr->src_rect.w, spr->src_rect.h);
            on_screen_size *= g::camera.zoom;

            SDL_FRect on_screen_rect = Vec2Vec2toRect(on_screen_position, on_screen_size);

            SDL_RenderCopyF(g::renderer, spr->texture, &spr->src_rect, &on_screen_rect);

            // debug lines -------------------------------------------
            SDL_SetRenderDrawColor(g::renderer, 255, 0, 0, 255);
            SDL_RenderDrawRectF(g::renderer, &on_screen_rect);
            int a = 10;
            SDL_FRect spr_origin_rect = Vec2Vec2toRect(
                on_screen_position + (spr->origin * g::camera.zoom),
                glm::ivec2(a,a)
            );
            spr_origin_rect.x -= a/2; spr_origin_rect.y -= a/2;
            SDL_RenderFillRectF(g::renderer, &spr_origin_rect);
        }

        // show mouse cursor
        SDL_SetRenderDrawColor(g::renderer, 0, 255, 255, 255);
        SDL_Rect viewport_mouse_rect = (SDL_Rect) {g::viewport.mouse_position.x-2, g::viewport.mouse_position.y-2, 4, 4};
        SDL_RenderFillRect(g::renderer, &viewport_mouse_rect);

        SDL_SetRenderTarget(g::renderer, NULL); // switch target back to renderer (window)

        SDL_FRect viewport_rect = (SDL_FRect){
            g::viewport.position.x, 
            g::viewport.position.y, 
            (float)g::viewport.size.x, 
            (float)g::viewport.size.y
        };

        // send viewport texture to window renderer
        SDL_RenderCopyF(g::renderer, viewport_texture, NULL, &viewport_rect);
        SDL_DestroyTexture(viewport_texture);

        // red viewport outline
        SDL_SetRenderDrawColor(g::renderer, 255, 0, 0, 255);
        SDL_RenderDrawRectF(g::renderer, &viewport_rect);

        // present the renderer back buffer
        SDL_RenderPresent(g::renderer);

        // debug
        SDL_SetWindowTitle(g::window, (std::to_string(g::time.DeltaTime()) + ", " + std::to_string(g::time.FPS())).c_str());

        // update delta time
        g::time.UpdateLast(SDL_GetTicks());
    }
    std::cout << std::endl << "Main loop ended." << std::endl;

    SDL_DestroyTexture(viewport_texture);
    DestroyAllSprites(&sprites);
}
