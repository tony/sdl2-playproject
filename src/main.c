#include <assert.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>
#include "main.h"

const int SCREEN_WIDTH = 630;
const int SCREEN_HEIGHT = 480;

const int BOTTOM_VIEWPORT_X = 0;
const int BOTTOM_VIEWPORT_Y = SCREEN_HEIGHT * .9;
const int BOTTOM_VIEWPORT_W = SCREEN_WIDTH;
const int BOTTOM_VIEWPORT_H = SCREEN_HEIGHT * .1;

const SDL_Rect BOTTOM_VIEWPORT_RECT = {
    .x = BOTTOM_VIEWPORT_X,
    .y = BOTTOM_VIEWPORT_Y,
    .w = BOTTOM_VIEWPORT_W,
    .h = BOTTOM_VIEWPORT_H
};

const int MAIN_VIEWPORT_X = 0;
const int MAIN_VIEWPORT_Y = 0;
const int MAIN_VIEWPORT_W = SCREEN_WIDTH;
const int MAIN_VIEWPORT_H = SCREEN_HEIGHT * .9;

const SDL_Rect MAIN_VIEWPORT_RECT = {
    .x = MAIN_VIEWPORT_X,
    .y = MAIN_VIEWPORT_Y,
    .w = MAIN_VIEWPORT_W,
    .h = MAIN_VIEWPORT_H
};

const int SHOOTING_DELAY = 200;

const int HERO_SPRITE_W = 30;
const int HERO_SPRITE_H = 30;

bool
game_load_textures(SDL_Texture** bgTexture, SDL_Renderer* renderer)
{
    bool success = true;

    *bgTexture = texture_load("resources/continents.png", renderer);
    if (bgTexture == NULL) {
        success = false;
    }

    return success;
}

void 
game_close(SDL_Texture* bgTexture, SDL_Renderer* renderer, SDL_Window* window) 
{
    SDL_DestroyTexture(bgTexture);
    bgTexture = NULL;

    renderer = NULL;
    window = NULL;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();
    SDL_Quit();
}

void
draw_button(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect button = {
        .x = 0,
        .y = 0,
        .w = 50,
        .h = 50
    };

    const int button_unit = button.w * 0.05;

    SDL_RenderFillRect(renderer, &button);

    SDL_Rect blackFills[] = {
        {0, 0, button.w - 1, 1 * button_unit},
        {0, 1 * button_unit, button.w - 2, 1 * button_unit}
    };

    SDL_Rect goldFills[] = {
        {0, 0, button.w - 1, 1 * button_unit},
        {0, 1 * button_unit, button.w - 2, 1 * button_unit}
    };

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // black
    SDL_RenderFillRects(renderer, blackFills, 2);

    SDL_SetRenderDrawColor(renderer, 137, 78, 0, 255);  // gold
    SDL_RenderFillRects(renderer, goldFills, 2);

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
}

void
game_callback(const SDL_Event* e, bool* quit) 
{
    if(e->type == SDL_QUIT) {
        *quit = true;
    } else if (e->type == SDL_KEYDOWN) {
        switch(e->key.keysym.sym) {
        case SDLK_ESCAPE:
            *quit = true;
            break;

        case SDLK_c:
            if (e->key.keysym.mod & KMOD_CTRL) {
                *quit = true;
            }
            break;
        }
    }
}

int
main(void) {
    bool quit = false;
    int imgFlags = IMG_INIT_PNG;
    SDL_Event e;

    Hero hero = {
        .spriteSheet = NULL,
        .position = { 0, 0, 30, 30 },
        .state = HERO_STATE_DEFAULT
    };
    SDL_Renderer* renderer;
    SDL_Window* window;
    SDL_Texture* bgTexture = NULL;

    Boomerangs boomerangs;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fatal("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }

    if (!(IMG_Init(imgFlags) & imgFlags)) {
        fatal("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
    }

    SDL_CreateWindowAndRenderer(
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN,
        &window,
        &renderer
    );

    if (window == NULL) {
        fatal("Window could not be created!  SDL_Error: %s\n", SDL_GetError());
    } else if (renderer == NULL) {
        fatal("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
    }

    if(!game_load_textures(&bgTexture, renderer)) {
        fatal("Failed to load media!\n");
    } else if(!hero_load_textures(&hero, renderer)) {
        fatal("Failed to load hero media!\n");
    } else if(!boomerangs_init(&boomerangs, renderer)) {
        fatal("Failed to load boomerang media!\n");
    }

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    while (!quit) {
        SDL_RenderClear(renderer);
        SDL_RenderSetViewport(renderer, &BOTTOM_VIEWPORT_RECT);
        draw_button(renderer);
        SDL_RenderSetViewport(renderer, &MAIN_VIEWPORT_RECT);
        SDL_RenderCopy(renderer, bgTexture, NULL, NULL);
        while (SDL_PollEvent(&e) != 0) {
            game_callback(&e, &quit);
        }
        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
        hero_callback(&hero, &boomerangs, currentKeyStates);
        boomerangs_update(&boomerangs);
        boomerangs_draw(&boomerangs, renderer);
        SDL_RenderCopy(renderer, hero.spriteSheet, &hero.HeroState[hero.state], &hero.position);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    hero_delete(&hero);
    game_close(bgTexture, renderer, window);

    return 0;
}
