/* Copyright 2016 Tony Narlock. All rights reserved. */
#include <memory>
#include "config.h"
#include "util.h"

std::shared_ptr<SDL2pp::Texture> DrawText(
    const std::string& text,
    const SDL2pp::Point& position,
    const std::shared_ptr<SDL2pp::Font>& font,
    const std::unique_ptr<SDL2pp::Renderer>& renderer,
    bool underline = true) {
  SDL_Color text_fg_color{255, 255, 255, 255};
  SDL_Color text_shadow_color{0, 0, 0, 255};

  auto target1 = std::make_shared<SDL2pp::Texture>(
      *renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET,
      SCREEN_RECT.w, SCREEN_RECT.h);
  target1->SetBlendMode(SDL_BLENDMODE_BLEND);
  renderer->SetTarget(*target1);
  renderer->Clear();
  renderer->SetDrawBlendMode(SDL_BLENDMODE_BLEND);

  if (underline) {
    auto message_shadow(font->RenderText_Blended(text, text_shadow_color));
    auto message_texture_shadow(SDL2pp::Texture(*renderer, message_shadow));
    SDL2pp::Rect message_shadow_rect = {position.x + 1, position.y + 1,
                                        message_shadow.GetWidth(),
                                        message_shadow.GetHeight()};
    renderer->Copy(message_texture_shadow, SDL2pp::NullOpt,
                   message_shadow_rect);
  }

  auto message(font->RenderText_Blended(text, text_fg_color));
  auto message_texture(SDL2pp::Texture(*renderer, message));
  SDL2pp::Rect message_rect = {position.x, position.y, message.GetWidth(),
                               message.GetHeight()};
  renderer->Copy(message_texture, SDL2pp::NullOpt, message_rect);

  renderer->SetTarget();
  return target1;
}

int RandInt(int lo, int hi) {
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<int> dist(lo, hi);
  return dist(mt);
}
