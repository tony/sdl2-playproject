#include <assert.h>
#include <stdbool.h>
#include "main.h"

const int SCREEN_WIDTH = 630;
const int SCREEN_HEIGHT = 480;

const SDL_Rect BOTTOM_VIEWPORT_RECT = {
    .x = 0,
    .y = SCREEN_HEIGHT * .9,
    .w = SCREEN_WIDTH,
    .h = SCREEN_HEIGHT * .1
};

const SDL_Rect MAIN_VIEWPORT_RECT = {
    .x = 0,
    .y = 0,
    .w = SCREEN_WIDTH,
    .h = SCREEN_HEIGHT * .9 
};

const int SHOOTING_DELAY = 200;

const int HERO_SPRITE_W = 30;
const int HERO_SPRITE_H = 30;

TTF_Font* font = NULL;

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
        .state = HERO_STATE_DEFAULT,
        .stats = { 100, 100, 8, 8 }
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

    window = SDL_CreateWindow(
        "sdl2-playproject",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_RESIZABLE
    );

    renderer = SDL_CreateRenderer(window,
        -1,
        SDL_RENDERER_ACCELERATED
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
    } else if ( TTF_Init() == -1 ) {
        fatal("Font engine failed to initialize.\n");
    }

    // load fonts
    font = TTF_OpenFont("resources/fonts/TerminusTTF-Bold-4.39.ttf", 36);
    if (font == NULL) {
        fatal("Error loading font");
    }

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    while (!quit) {
        SDL_RenderClear(renderer);
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
        char* herotext;
        sprintf(herotext, "health %d / %d", hero.stats.current_hp, hero.stats.hp);
        draw_text(herotext, 0, 0, font, renderer);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    hero_delete(&hero);
    game_close(bgTexture, renderer, window);

    return 0;
}
