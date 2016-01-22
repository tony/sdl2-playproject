#include <stdbool.h>
#include <errno.h>
#include "main.h"

/* Log a critical error with error string and die. */
__dead void fatal(const char* msg, ...) {
  char* fmt;
  va_list ap;

  va_start(ap, msg);
  if (asprintf(&fmt, "fatal: %s: %s", msg, strerror(errno)) == -1)
    exit(1);
  exit(1);
}

void draw_text(const char* text,
               const int x,
               const int y,
               TTF_Font* font,
               std::shared_ptr<SDL_Renderer> renderer) {
  SDL_Color textForegroundColor;
  textForegroundColor.r = 255, textForegroundColor.g = 255,
  textForegroundColor.b = 255, textForegroundColor.a = 255;
  SDL_Color textShadowColor;
  textShadowColor.r = 0, textShadowColor.g = 0, textShadowColor.b = 0,
  textShadowColor.a = 255;
  SDL_Surface* message = nullptr;
  SDL_Surface* message_shadow = nullptr;
  SDL_Texture* message_texture = nullptr;
  SDL_Texture* message_texture_shadow = nullptr;

  message = TTF_RenderText_Solid(font, text, textForegroundColor);
  message_shadow = TTF_RenderText_Solid(font, text, textShadowColor);
  message_texture = SDL_CreateTextureFromSurface(renderer.get(), message);
  message_texture_shadow =
      SDL_CreateTextureFromSurface(renderer.get(), message_shadow);

  SDL_Rect message_shadow_rect = {x + 2, y + 2, message_shadow->w,
                                  message_shadow->h};
  SDL_RenderCopy(renderer.get(), message_texture_shadow, NULL, &message_shadow_rect);

  SDL_Rect message_rect = {x, y, message->w, message->h};
  SDL_RenderCopy(renderer.get(), message_texture, NULL, &message_rect);

  SDL_FreeSurface(message);
  SDL_FreeSurface(message_shadow);
  SDL_DestroyTexture(message_texture);
  SDL_DestroyTexture(message_texture_shadow);
  message = nullptr;
  message_shadow = nullptr;
  message_texture = nullptr;
  message_texture_shadow = nullptr;
}

SDL_Texture* texture_load(const char* path, std::shared_ptr<SDL_Renderer> renderer) {
  SDL_Texture* newTexture = NULL;
  SDL_Surface* loadedSurface = IMG_Load(path);

  if (loadedSurface == NULL) {
    fatal("Unable to load image %s! SDL_image Error: %s\n", path,
          IMG_GetError());
  } else {
    SDL_SetColorKey(loadedSurface, SDL_TRUE,
                    SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

    newTexture = SDL_CreateTextureFromSurface(renderer.get(), loadedSurface);
    if (newTexture == NULL) {
      fatal("Unable to create texture %s! SDL Error: %s\n", path,
            SDL_GetError());
    }

    SDL_FreeSurface(loadedSurface);
  }
  return newTexture;
}

void apply_surface(int x,
                   int y,
                   SDL_Surface* source,
                   SDL_Surface* destination,
                   SDL_Rect* clip) {
  // Holds offsets
  SDL_Rect offset;

  // Get offsets
  offset.x = x;
  offset.y = y;

  // Blit
  SDL_BlitSurface(source, clip, destination, &offset);
}
