#include "game.h"
#include <cerrno>

/* Log a critical error with error string and die. */
__dead void fatal(const char* msg, ...) {
  char* fmt;
  va_list ap;

  va_start(ap, msg);
  if (asprintf(&fmt, "fatal: %s: %s", msg, strerror(errno)) == -1) {
    exit(1);
}
  exit(1);
}

void draw_text(const char* text,
               const int x,
               const int y,
               TTF_Font* font,
               SDL2pp::Renderer& renderer) {
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
  message_texture = SDL_CreateTextureFromSurface(renderer.Get(), message);
  message_texture_shadow =
      SDL_CreateTextureFromSurface(renderer.Get(), message_shadow);

  SDL_Rect message_shadow_rect = {x + 2, y + 2, message_shadow->w,
                                  message_shadow->h};
  SDL_RenderCopy(renderer.Get(), message_texture_shadow, nullptr,
                 &message_shadow_rect);

  SDL_Rect message_rect = {x, y, message->w, message->h};
  SDL_RenderCopy(renderer.Get(), message_texture, nullptr, &message_rect);

  SDL_FreeSurface(message);
  SDL_FreeSurface(message_shadow);
  SDL_DestroyTexture(message_texture);
  SDL_DestroyTexture(message_texture_shadow);
  message = nullptr;
  message_shadow = nullptr;
  message_texture = nullptr;
  message_texture_shadow = nullptr;
}

char* get_full_path(const char* path) {
  auto full_path = SDL_GetBasePath();
  return strcat(full_path, path);
}
