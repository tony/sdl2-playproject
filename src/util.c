#include <stdbool.h>
#include <errno.h>
#include <SDL.h>
#include <SDL_image.h>
#include "main.h"

extern App app;

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
