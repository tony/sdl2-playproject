/* Copyright 2016 Tony Narlock. All rights reserved. */
#ifndef SRC_UTIL_H_
#define SRC_UTIL_H_

#include <SDL2pp/Font.hh>
#include <SDL2pp/SDL2pp.hh>
#include <algorithm>
#include <memory>
#include <string>
#include <random>

template <typename T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi) {
  return std::max(lo, std::min(v, hi));
}

std::shared_ptr<SDL2pp::Texture> DrawText(
    const std::string& text,
    const SDL2pp::Point& position,
    const std::shared_ptr<SDL2pp::Font>& font,
    const std::unique_ptr<SDL2pp::Renderer>& renderer,
    bool underline);

int RandInt(int lo = 0, int hi = RAND_MAX);

#endif  // SRC_UTIL_H_
