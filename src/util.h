/* Copyright 2016 Tony Narlock. All rights reserved. */
#ifndef SRC_UTIL_H_
#define SRC_UTIL_H_

#include <SDL2pp/Font.hh>
#include <SDL2pp/SDL2pp.hh>
#include <algorithm>
#include <memory>
#include <string>

template <typename T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi) {
  return std::max(lo, std::min(v, hi));
}

std::shared_ptr<SDL2pp::Texture> DrawText(
    const std::string text,
    const int x,
    const int y,
    const std::shared_ptr<SDL2pp::Font>& font,
    const std::unique_ptr<SDL2pp::Renderer>& renderer,
    bool underline);

std::string GetFullPath(const char* path);
#endif  // SRC_UTIL_H_
