#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>
#include "main.h"

const int SCREEN_WIDTH = 630;
const int SCREEN_HEIGHT = 480;
const int MAX_HERO_MOVEMENT = 5;

Hero hero = {
    .spriteSheet = NULL,
    .position = { 0, 0, 30, 30 },
    .state = HERO_STATE_DEFAULT
};

App app = {
    .window = NULL,
};

/* Log a critical error with error string and die. */
__dead void
fatal(const char *msg, ...)
{
    char    *fmt;
    va_list  ap;

    va_start(ap, msg);
    if (asprintf(&fmt, "fatal: %s: %s", msg, strerror(errno)) == -1)
        exit(1);
    exit(1);
}

SDL_Texture*
texture_load(const char* path)
{
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path);

    if (loadedSurface == NULL) {
        fatal("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
    } else {
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

        newTexture = SDL_CreateTextureFromSurface(app.renderer, loadedSurface);
        if (newTexture == NULL) {
            fatal("Unable to create texture %s! SDL Error: %s\n", path, SDL_GetError());
        }

        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

bool
app_load_textures(void)
{
    bool success = true;

    app.bgTexture = texture_load("resources/continents.png");
    if (app.bgTexture == NULL) {
        printf("Failed to load image!\n");
        success = false;
    }

    return success;
}

void 
app_close(void) 
{
    SDL_DestroyTexture(app.bgTexture);
    app.bgTexture = NULL;

    SDL_DestroyTexture(hero.spriteSheet);
    hero.spriteSheet = NULL;

    app.renderer = NULL;
    app.window = NULL;
    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);

    IMG_Quit();
    SDL_Quit();
}

void
app_callback(SDL_Event* e, bool* quit) 
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
        &app.window,
        &app.renderer
    );

    if (app.window == NULL) {
        fatal("Window could not be created!  SDL_Error: %s\n", SDL_GetError());
    } else if (app.renderer == NULL) {
        fatal("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
    }

    if(!app_load_textures()) {
        fatal("Failed to load media!\n");
    } else if(!hero_load_textures()) {
        fatal("Failed to hero media!\n");
    }

    SDL_SetRenderDrawColor(app.renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    while (!quit) {
        SDL_RenderClear(app.renderer);
        SDL_RenderCopy(app.renderer, app.bgTexture, NULL, NULL);
        while (SDL_PollEvent(&e) != 0) {
            app_callback(&e, &quit);
            hero_callback(&e);
        }
        SDL_RenderCopy(app.renderer, hero.spriteSheet, &hero.HeroState[hero.state], &hero.position);

        SDL_RenderPresent(app.renderer);
    }

    app_close();
    return 0;
}
