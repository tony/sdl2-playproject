/* Copyright 2016 Tony Narlock. All rights reserved. */
#ifndef SRC_COMPONENTS_GEOMETRY_H_
#define SRC_COMPONENTS_GEOMETRY_H_
#include <SDL2pp/SDL2pp.hh>
struct Geometry {
  Geometry(SDL2pp::Point position,
           SDL2pp::Point direction,
           SDL2pp::Point size,
           double angle = 0.0,
           unsigned int scale = 1,
           int flip = 0)
      : position(position),
        direction(direction),
        size(size * scale),
        angle(angle),
        scale(scale),
        flip(flip) {}

  SDL2pp::Point position;
  SDL2pp::Point direction;
  SDL2pp::Point size;
  SDL2pp::Rect GetArea() const {
    return SDL2pp::Rect{position.x, position.y, size.x, size.y};
  };
  float angle = 0.0;
  unsigned int scale;
  int flip = 0;
};
#endif  // SRC_COMPONENTS_GEOMETRY_H_
