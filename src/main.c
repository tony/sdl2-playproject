#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>
#include "main.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int MAX_HERO_MOVEMENT = 5;

Hero hero = {
    .texture = NULL,
    .state = HERO_STATE_DEFAULT,
    .position = { 150, 50, -1, -1 },
    .HeroState = NULL
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
        printf("Unable to load iamge %s! SDL_image Error: %s\n", path, IMG_GetError());
    } else {
        //Color key image
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

        newTexture = SDL_CreateTextureFromSurface(app.renderer, loadedSurface);
        if (newTexture == NULL) {
            printf("Unable to create texture %s! SDL Error: %s\n", path, SDL_GetError());
        }

        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

void
texture_render(SDL_Texture* texture, int x, int y, int w, int h)
{
    if (w < 1 || h < 1) {
        SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    }
    SDL_Rect renderQuad = { x, y, w, h };
    SDL_RenderCopy(app.renderer, texture, NULL, &renderQuad);
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

void
hero_callback(SDL_Event* e)
{
    if (e->type == SDL_KEYDOWN) {
        switch(e->key.keysym.sym) {
        case SDLK_UP:
            hero.texture = hero.HeroState[ HERO_STATE_WALK_UP ];
            hero_throttle_alter(&hero.velocity, 0, -1);
            break;

        case SDLK_DOWN:
            hero.texture = hero.HeroState[ HERO_STATE_WALK_DOWN ];
            hero_throttle_alter(&hero.velocity, 0, 1);
            break;

        case SDLK_LEFT:
            hero.texture = hero.HeroState[ HERO_STATE_WALK_LEFT ];
            hero_throttle_alter(&hero.velocity, -1, 0);
            break;

        case SDLK_RIGHT:
            hero.texture = hero.HeroState[ HERO_STATE_WALK_RIGHT ];
            hero_throttle_alter(&hero.velocity, 1, 0);
            break;

        default:
            hero.texture = hero.HeroState[ HERO_STATE_DEFAULT ];
            break;
        }
    }

    hero_throttle_acceleration(&hero.velocity);
    hero_calculate_position(&hero.position, &hero.velocity);
}

void hero_throttle_alter(SDL_Point* velocity, int x, int y) {
    if (abs((*velocity).x + x) <= MAX_HERO_MOVEMENT) {
        (*velocity).x += x;
    }

    if (abs((*velocity).y + y) <= MAX_HERO_MOVEMENT) {
        (*velocity).y += y;
    }
}

void hero_throttle_acceleration(SDL_Point* velocity)
{
    if (abs((*velocity).x) > MAX_HERO_MOVEMENT) {
        (*velocity).x = (*velocity).x > 0 ? (*velocity).x-- : (*velocity).x++;
    }
    if (abs((*velocity).y) > MAX_HERO_MOVEMENT) {
        (*velocity).y = (*velocity).y > 0 ? (*velocity).y-- : (*velocity).y++;
    }
}

void
hero_calculate_position(SDL_Rect* position, SDL_Point* velocity)
{
    (*position).x += (*velocity).x;
    (*position).y += (*velocity).y;
}

bool
hero_load_textures(void)
{
    bool success = true;

    hero.HeroState[ HERO_STATE_DEFAULT ] = texture_load("resources/elliot/Down_0.png");
    hero.HeroState[ HERO_STATE_WALK_UP ] = texture_load("resources/elliot/Up_0.png");
    hero.HeroState[ HERO_STATE_WALK_DOWN ] = texture_load("resources/elliot/Down_0.png");
    hero.HeroState[ HERO_STATE_WALK_LEFT ] = texture_load("resources/elliot/Left_0.png");
    hero.HeroState[ HERO_STATE_WALK_RIGHT ] = texture_load("resources/elliot/Right_0.png");

    for (int i=HERO_STATE_DEFAULT; i < HERO_STATE_TOTAL; ++i) {
        if (hero.HeroState[i] == NULL) {
            printf("Failed to load default image!\n");
            success = false;
        }
    }
    return success;
}

int main(void) {
    bool quit = false;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fatal("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    } else {
        SDL_CreateWindowAndRenderer(
                SCREEN_WIDTH,
                SCREEN_HEIGHT,
                SDL_WINDOW_SHOWN,
                &app.window,
                &app.renderer
       );

        if (app.window == NULL) {
            fatal("Window could not be created!  SDL_Error: %s\n", SDL_GetError());
        }
        if (app.renderer == NULL) {
                fatal("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        }
        SDL_SetRenderDrawColor(app.renderer, 0xFF, 0xFF, 0xFF, 0xFF);

        int imgFlags = IMG_INIT_PNG;
        if (!(IMG_Init(imgFlags) & imgFlags)) {
            fatal("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        }
    }

    if(!app_load_textures()) {
        fatal("Failed to load media!\n");
    } else if(!hero_load_textures()) {
        fatal("Failed to hero media!\n");
    } else {
        //Apply the image
        SDL_Event e;

        hero.texture = hero.HeroState[ HERO_STATE_DEFAULT ];
        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                app_callback(&e, &quit);
                hero_callback(&e);
            }

            SDL_RenderClear(app.renderer);

            SDL_RenderCopy(app.renderer, app.bgTexture, NULL, NULL);

            texture_render(hero.texture, hero.position.x, hero.position.y, -1, -1);

            SDL_RenderPresent(app.renderer);
        }
    }

    app_close();
    return 0;
}
