#include "config.h"
#include "util.h"
#include <memory>

SDL2pp::Texture DrawText(const std::string text,
                         const int x,
                         const int y,
                         const std::shared_ptr<SDL2pp::Font>& font,
                         const std::unique_ptr<SDL2pp::Renderer>& renderer) {
  SDL_Color text_fg_color{255, 255, 255, 255};
  SDL_Color text_shadow_color{0, 0, 0, 255};

  SDL2pp::Texture target1(*renderer, SDL_PIXELFORMAT_ARGB8888,
                          SDL_TEXTUREACCESS_TARGET, SCREEN_RECT.w,
                          SCREEN_RECT.h);
  renderer->SetTarget(target1);
  renderer->Clear();
  renderer->SetDrawBlendMode(SDL_BLENDMODE_BLEND);

  auto message(font->RenderText_Blended(text, text_fg_color));
  auto message_shadow(font->RenderText_Blended(text, text_shadow_color));

  auto message_texture(SDL2pp::Texture(*renderer, message));
  auto message_texture_shadow(SDL2pp::Texture(*renderer, message_shadow));

  message_texture_shadow.SetBlendMode(SDL_BLENDMODE_BLEND);
  message_texture.SetBlendMode(SDL_BLENDMODE_BLEND);

  SDL2pp::Rect message_shadow_rect = {x + 1, y + 1, message_shadow.GetWidth(),
                                      message_shadow.GetHeight()};
  SDL2pp::Rect message_rect = {x, y, message.GetWidth(), message.GetHeight()};

  renderer->Copy(message_texture_shadow, SDL2pp::NullOpt, message_shadow_rect);
  renderer->Copy(message_texture, SDL2pp::NullOpt, message_rect);

  renderer->SetTarget();
  return target1;
}

std::string GetFullPath(const char* path) {
  auto base_path = SDL_GetBasePath();
  std::string full_path = std::string(base_path) + std::string(path);
  return full_path;
}
