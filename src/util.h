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
              const std::shared_ptr<SDL2pp::Font>& font,
              const std::shared_ptr<SDL2pp::Renderer>& renderer);

std::string GetFullPath(const char* path);
