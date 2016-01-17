#include <stdbool.h>
#include <errno.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "main.h"

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

void
draw_text(const char* text, const int x, const int y, const TTF_Font* font, SDL_Renderer* renderer) 
{
    const SDL_Color textForegroundColor = { 255, 255, 255 };
    const SDL_Color textShadowColor = { 0, 0, 0 };
    SDL_Surface *message = NULL;
    SDL_Surface *message_shadow = NULL;
    SDL_Texture *message_texture = NULL;
    SDL_Texture *message_texture_shadow = NULL;

    message = TTF_RenderText_Solid(font, text, textForegroundColor);
    message_shadow = TTF_RenderText_Solid(font, text, textShadowColor);
    message_texture = SDL_CreateTextureFromSurface(renderer, message);
    message_texture_shadow = SDL_CreateTextureFromSurface(renderer, message_shadow);

    SDL_RenderCopy(renderer, message_texture_shadow, NULL, &(SDL_Rect){ 2, 2, message_shadow->w, message_shadow->h });
    SDL_RenderCopy(renderer, message_texture, NULL, &(SDL_Rect){ 0, 0, message->w, message->h });

}


SDL_Texture*
texture_load(const char* path, SDL_Renderer* renderer)
{
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path);

    if (loadedSurface == NULL) {
        fatal("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
    } else {
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == NULL) {
            fatal("Unable to create texture %s! SDL Error: %s\n", path, SDL_GetError());
        }

        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

void
apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip)
{
    //Holds offsets
    SDL_Rect offset;
    
    //Get offsets
    offset.x = x;
    offset.y = y;
    
    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}
