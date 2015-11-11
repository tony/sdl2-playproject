#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>
#include "main.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

App app = {
    .window = NULL,
    .screenSurface = NULL,
    .keyPressSurfaces = NULL,
    .currentSurface = NULL
};


bool initWindow() {
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
            app.screenSurface = SDL_GetWindowSurface( app.window );
            SDL_FillRect( app.screenSurface, NULL, SDL_MapRGB( app.screenSurface->format, 0xFF, 0xFF, 0xFF ) );
            SDL_UpdateWindowSurface(app.window);
        }
    }

    return success;
}

SDL_Surface* loadSurface( const char* path )
{
    //The final optimized image
    SDL_Surface* optimizedSurface = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError() );
    }
    else
    {
        //Convert surface to screen format
        optimizedSurface = SDL_ConvertSurface( loadedSurface, app.screenSurface->format, 0 );
        if( optimizedSurface == NULL )
        {
            printf( "Unable to optimize image %s! SDL Error: %s\n", path, SDL_GetError() );
        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    return optimizedSurface;
}

bool loadMedia() {
    bool success = true;

    app.keyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ] = loadSurface( "resources/elliot/Down_0.png" );
    if( app.keyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ] == NULL )
    {
        printf( "Failed to load default image!\n" );
        success = false;
    }

    //Load up surface
    app.keyPressSurfaces[ KEY_PRESS_SURFACE_UP ] = loadSurface( "resources/elliot/Up_0.png" );
    if( app.keyPressSurfaces[ KEY_PRESS_SURFACE_UP ] == NULL )
    {
        printf( "Failed to load up image!\n" );
        success = false;
    }

    //Load down surface
    app.keyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ] = loadSurface( "resources/elliot/Down_0.png" );
    if( app.keyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ] == NULL )
    {
        printf( "Failed to load down image!\n" );
        success = false;
    }

    //Load left surface
    app.keyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ] = loadSurface( "resources/elliot/Left_0.png" );
    if( app.keyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ] == NULL )
    {
        printf( "Failed to load left image!\n" );
        success = false;
    }

    //Load right surface
    app.keyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ] = loadSurface( "resources/elliot/Right_0.png" );
    if( app.keyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ] == NULL )
    {
        printf( "Failed to load right image!\n" );
        success = false;
    }
    return success;
}

void appClose() {
    SDL_DestroyWindow(app.window);
    app.window = NULL;

    SDL_Quit();
}

void appMainLoop(SDL_Event* e, bool* quit) {
    if(e->type == SDL_QUIT) {
        *quit = true;
    } else if (e->type == SDL_KEYDOWN) {

        switch( e->key.keysym.sym )
        {
            case SDLK_UP:
            app.currentSurface = app.keyPressSurfaces[ KEY_PRESS_SURFACE_UP ];
            break;

            case SDLK_DOWN:
            app.currentSurface = app.keyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ];
            break;

            case SDLK_LEFT:
            app.currentSurface = app.keyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ];
            break;

            case SDLK_RIGHT:
            app.currentSurface = app.keyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ];
            break;

            default:
            app.currentSurface = app.keyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];
            break;
        }
    }
}

int main() {

    bool quit = false;

    if( !initWindow() ) {
        printf( "Failed to initialize!\n" );
    } else {
        //Load media
        if( !loadMedia() ) {
            printf( "Failed to load media!\n" );
        } else {
            //Apply the image
            SDL_Event e;

            app.currentSurface = app.keyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];
            while (!quit) {
                while (SDL_PollEvent( &e ) != 0) {
                    appMainLoop(&e, &quit);
                }

				SDL_Rect stretchRect;
				stretchRect.x = 0;
				stretchRect.y = 0;
				stretchRect.w = SCREEN_WIDTH;
				stretchRect.h = SCREEN_HEIGHT;
				SDL_BlitScaled( app.currentSurface, NULL, app.screenSurface, &stretchRect );
            
                //Update the surface
                SDL_UpdateWindowSurface( app.window );
            }
        }

    }

    appClose();
    return 0;
}
