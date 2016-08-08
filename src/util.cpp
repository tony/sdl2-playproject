#include "util.h"

void DrawText(const std::string text,
              const int x,
              const int y,
              SDL2pp::Font& font,
              SDL2pp::Renderer& renderer) {
  SDL_Color textForegroundColor;
  textForegroundColor.r = 255, textForegroundColor.g = 255,
  textForegroundColor.b = 255, textForegroundColor.a = 255;
  SDL_Color textShadowColor;
  textShadowColor.r = 0, textShadowColor.g = 0, textShadowColor.b = 0,
  textShadowColor.a = 255;

  auto message = font.RenderText_Solid(text, textForegroundColor);
  auto message_shadow = font.RenderText_Solid(text.c_str(), textShadowColor);
  auto message_texture = SDL2pp::Texture(renderer, message);
  auto message_texture_shadow = SDL2pp::Texture(renderer, message_shadow);

  SDL2pp::Rect message_shadow_rect = {x + 1, y + 1, message_shadow.GetWidth(),
                                      message_shadow.GetHeight()};
  renderer.Copy(message_texture_shadow, SDL2pp::NullOpt, message_shadow_rect);

  SDL2pp::Rect message_rect = {x, y, message.GetWidth(), message.GetHeight()};
  renderer.Copy(message_texture, SDL2pp::NullOpt, message_rect);
}

std::string GetFullPath(const char* path) {
  auto base_path = SDL_GetBasePath();
  std::string full_path = std::string(base_path) + std::string(path);
  return full_path;
}
