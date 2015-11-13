#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>
#include "main.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

Sprite heroSprite = {
    .texture = NULL
};

Hero hero = {
    .sprite = &heroSprite,
    .state = HERO_STATE_DEFAULT,
    .HeroState = NULL
};

App app = {
    .window = NULL,
    .hero = &hero
};


bool
app_init(void)
{
    bool success = true;
    if (SDL_Init( SDL_INIT_VIDEO ) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    } else {
        SDL_CreateWindowAndRenderer(
                SCREEN_WIDTH,
                SCREEN_HEIGHT,
                SDL_WINDOW_SHOWN,
                &app.window,
                &app.renderer
        );

        if (app.window == NULL) {
            printf("Window could not be created!  SDL_Error: %s\n", SDL_GetError() );
            success = false;
        } else if (app.renderer == NULL) {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
        } else {
            SDL_SetRenderDrawColor(app.renderer, 0xFF, 0xFF, 0xFF, 0xFF);

            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) & imgFlags)) {
                printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                success = false;
            }
        }
    }

    return success;
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
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

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
app_load_media(void)
{
    bool success = true;

    app.hero->HeroState[ HERO_STATE_DEFAULT ] = texture_load( "resources/elliot/Down_0.png" );
    app.hero->HeroState[ HERO_STATE_WALK_UP ] = texture_load( "resources/elliot/Up_0.png" );
    app.hero->HeroState[ HERO_STATE_WALK_DOWN ] = texture_load( "resources/elliot/Down_0.png" );
    app.hero->HeroState[ HERO_STATE_WALK_LEFT ] = texture_load( "resources/elliot/Left_0.png" );
    app.hero->HeroState[ HERO_STATE_WALK_RIGHT ] = texture_load( "resources/elliot/Right_0.png" );

    for (int i=HERO_STATE_DEFAULT; i <= HERO_STATE_TOTAL; ++i) {
        if (app.hero->HeroState[i] == NULL) {
            printf( "Failed to load default image!\n" );
            success = false;
        }
    }

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
        switch( e->key.keysym.sym ) {
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
        switch( e->key.keysym.sym ) {
        case SDLK_UP:
            app.hero->sprite->texture = app.hero->HeroState[ HERO_STATE_WALK_UP ];
            break;

        case SDLK_DOWN:
            app.hero->sprite->texture = app.hero->HeroState[ HERO_STATE_WALK_DOWN ];
            break;

        case SDLK_LEFT:
            app.hero->sprite->texture = app.hero->HeroState[ HERO_STATE_WALK_LEFT ];
            break;

        case SDLK_RIGHT:
            app.hero->sprite->texture = app.hero->HeroState[ HERO_STATE_WALK_RIGHT ];
            break;

        default:
            app.hero->sprite->texture = app.hero->HeroState[ HERO_STATE_DEFAULT ];
            break;
        }
    }
}

int main(void) {
    bool quit = false;

    if(!app_init()) {
        printf( "Failed to initialize!\n" );
    } else {
        //Load media
        if(!app_load_media()) {
            printf( "Failed to load media!\n" );
        } else {
            //Apply the image
            SDL_Event e;

            app.hero->sprite->texture = app.hero->HeroState[ HERO_STATE_DEFAULT ];
            while (!quit) {
                while (SDL_PollEvent( &e ) != 0) {
                    app_callback(&e, &quit);
                    hero_callback(&e);
                }

                SDL_RenderClear(app.renderer);

                SDL_RenderCopy(app.renderer, app.bgTexture, NULL, NULL);

                texture_render(app.hero->sprite->texture, 50, 50, -1, -1);

                SDL_RenderPresent(app.renderer);
            }
        }

    }

    app_close();
    return 0;
}
