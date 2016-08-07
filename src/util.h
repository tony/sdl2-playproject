#pragma once

#include <SDL2pp/Font.hh>
#include <SDL2pp/SDL2pp.hh>
#include <algorithm>

template <typename T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi) {
  return std::max(lo, std::min(v, hi));
}

void DrawText(const std::string text,
              const int x,
              const int y,
              SDL2pp::Font& font,
              SDL2pp::Renderer& renderer);

SDL2pp::Texture LoadImageAlpha(SDL2pp::Renderer& renderer,
                               std::string spritePath,
                               Uint8 r,
                               Uint8 g,
                               Uint8 b);

std::string GetFullPath(const char* path);
