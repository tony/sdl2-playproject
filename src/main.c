#include <assert.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>
#include "main.h"

const int SCREEN_WIDTH = 630;
const int SCREEN_HEIGHT = 480;
const int MAX_HERO_MOVEMENT = 5;

bool
game_load_textures(Game* game, SDL_Renderer* renderer)
{
    bool success = true;

    game->bgTexture = texture_load("resources/continents.png", renderer);
    if (game->bgTexture == NULL) {
        success = false;
    }

    return success;
}

void 
game_close(Game* game) 
{
    SDL_DestroyTexture(game->bgTexture);
    game->bgTexture = NULL;

    game->renderer = NULL;
    game->window = NULL;
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);

    IMG_Quit();
    SDL_Quit();
}

void
game_callback(Game* game, const SDL_Event* e, bool* quit) 
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

int main(void) {
    bool quit = false;
    int imgFlags = IMG_INIT_PNG;
    SDL_Event e;

    Hero hero = {
        .spriteSheet = NULL,
        .position = { 0, 0, 30, 30 },
        .state = HERO_STATE_DEFAULT
    };
    Game game = {
        .window = NULL,
    };

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
        &game.window,
        &game.renderer
    );

    if (game.window == NULL) {
        fatal("Window could not be created!  SDL_Error: %s\n", SDL_GetError());
    } else if (game.renderer == NULL) {
        fatal("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
    }

    if(!game_load_textures(&game, game.renderer)) {
        fatal("Failed to load media!\n");
    } else if(!hero_load_textures(&hero, game.renderer)) {
        fatal("Failed to load hero media!\n");
    } else if(!boomerangs_init(&boomerangs, game.renderer)) {
        fatal("Failed to load boomerang media!\n");
    }

    SDL_SetRenderDrawColor(game.renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    while (!quit) {
        SDL_RenderClear(game.renderer);
        SDL_RenderCopy(game.renderer, game.bgTexture, NULL, NULL);
        while (SDL_PollEvent(&e) != 0) {
            game_callback(&game, &e, &quit);
            hero_callback(&hero, &boomerangs, &e);
        }
        boomerangs_update(&boomerangs);
        boomerangs_draw(&boomerangs, game.renderer);
        SDL_RenderCopy(game.renderer, hero.spriteSheet, &hero.HeroState[hero.state], &hero.position);

        SDL_RenderPresent(game.renderer);
        SDL_Delay(16);
    }

    hero_delete(&hero);
    game_close(&game);

    return 0;
}
