#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>
#include "main.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

Hero hero = {
    .texture = NULL,
    .state = Stand,
    .StateTextures = NULL
};

App app = {
    .window = NULL,
    .hero = &hero,
};


bool initWindow(void) {
    bool success = true;
    if (SDL_Init( SDL_INIT_VIDEO ) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    } else {
        app.window = SDL_CreateWindow("SDL Tutorial",
                SDL_WINDOWPOS_UNDEFINED, 
                SDL_WINDOWPOS_UNDEFINED, 
                SCREEN_WIDTH,
                SCREEN_HEIGHT,
                SDL_WINDOW_SHOWN
        );

        if (app.window == NULL) {
            printf("Window could not be created!  SDL_Error: %s\n", SDL_GetError() );
            success = false;
        } else {
            app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED);
            if (app.renderer == NULL) {
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
    }

    return success;
}

SDL_Texture* loadTexture(const char* path) {
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

void renderTexture(SDL_Texture* texture, int x, int y, int w, int h) {
    if (w < 1 || h < 1) {
        SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    }
    SDL_Rect renderQuad = { x, y, w, h };
    SDL_RenderCopy(app.renderer, texture, NULL, &renderQuad);
}


bool appLoadMedia(void) {
    bool success = true;

    static int frame = 0;

    app.hero->StateTextures[ KEY_PRESS_SURFACE_DEFAULT ] = loadTexture( "resources/elliot/Down_0.png" );
    if( app.hero->StateTextures[ KEY_PRESS_SURFACE_DEFAULT ] == NULL )
    {
        printf( "Failed to load default image!\n" );
        success = false;
    }

    //Load up surface
    app.hero->StateTextures[ KEY_PRESS_SURFACE_UP ] = loadTexture( "resources/elliot/Up_0.png" );
    if( app.hero->StateTextures[ KEY_PRESS_SURFACE_UP ] == NULL )
    {
        printf( "Failed to load up image!\n" );
        success = false;
    }

    //Load down surface
    app.hero->StateTextures[ KEY_PRESS_SURFACE_DOWN ] = loadTexture( "resources/elliot/Down_0.png" );
    if( app.hero->StateTextures[ KEY_PRESS_SURFACE_DOWN ] == NULL )
    {
        printf( "Failed to load down image!\n" );
        success = false;
    }

    //Load left surface
    app.hero->StateTextures[ KEY_PRESS_SURFACE_LEFT ] = loadTexture( "resources/elliot/Left_0.png" );
    if( app.hero->StateTextures[ KEY_PRESS_SURFACE_LEFT ] == NULL )
    {
        printf( "Failed to load left image!\n" );
        success = false;
    }

    //Load right surface
    app.hero->StateTextures[ KEY_PRESS_SURFACE_RIGHT ] = loadTexture( "resources/elliot/Right_0.png" );
    if( app.hero->StateTextures[ KEY_PRESS_SURFACE_RIGHT ] == NULL )
    {
        printf( "Failed to load right image!\n" );
        success = false;
    }

    app.texture = loadTexture("resources/continents.png");
    if (app.texture == NULL) {
        printf("Failed to load image!\n");
        success = false;
    }

    return success;
}

void appClose(void) {
    SDL_DestroyTexture(app.texture);
    app.texture = NULL;

    app.renderer = NULL;
    app.window = NULL;
    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);

    IMG_Quit();
    SDL_Quit();
}

void handleAppEvent(SDL_Event* e, bool* quit) {
    if(e->type == SDL_QUIT) {
        *quit = true;
    } else if (e->type == SDL_KEYDOWN) {
        switch( e->key.keysym.sym ) {
            case  SDLK_ESCAPE:

            *quit = true;
            break;
        }
    }
}

void handleHeroEvent(SDL_Event* e) {
    if (e->type == SDL_KEYDOWN) {
        switch( e->key.keysym.sym ) {
            case SDLK_UP:
            app.hero->texture = app.hero->StateTextures[ KEY_PRESS_SURFACE_UP ];
            break;

            case SDLK_DOWN:
            app.hero->texture = app.hero->StateTextures[ KEY_PRESS_SURFACE_DOWN ];
            break;

            case SDLK_LEFT:
            app.hero->texture = app.hero->StateTextures[ KEY_PRESS_SURFACE_LEFT ];
            break;

            case SDLK_RIGHT:
            app.hero->texture = app.hero->StateTextures[ KEY_PRESS_SURFACE_RIGHT ];
            break;

            default:
            app.hero->texture = app.hero->StateTextures[ KEY_PRESS_SURFACE_DEFAULT ];
            break;
        }
    }
}

int main(void) {

    bool quit = false;

    if( !initWindow() ) {
        printf( "Failed to initialize!\n" );
    } else {
        //Load media
        if( !appLoadMedia() ) {
            printf( "Failed to load media!\n" );
        } else {
            //Apply the image
            SDL_Event e;

            app.hero->texture = app.hero->StateTextures[ KEY_PRESS_SURFACE_DEFAULT ];
            while (!quit) {
                while (SDL_PollEvent( &e ) != 0) {
                    handleAppEvent(&e, &quit);
                    handleHeroEvent(&e);
                }

                SDL_RenderClear(app.renderer);

                SDL_RenderCopy(app.renderer, app.texture, NULL, NULL);

                renderTexture(app.hero->texture, 50, 50, -1, -1);

                SDL_RenderPresent(app.renderer);
            }
        }

    }

    appClose();
    return 0;
}
