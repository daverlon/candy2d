#include "game.h"

#include "stdafx.h"
#include "debugutils.h"

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

    SDL_SetHint("SDL_RENDER_BATCHING", "1");

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
    return true;
}

void DestroySDL2() {
    std::cout << "Cleanup SDL2..." << std::endl;
    SDL_DestroyWindow(g::window);
    SDL_DestroyRenderer(g::renderer);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

int main() {
    if (!InitSDL2()) return 1;
    RunGame();
    DestroySDL2();
    return 0;
}