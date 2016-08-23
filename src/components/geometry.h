/* Copyright 2016 Tony Narlock. All rights reserved. */
#ifndef SRC_COMPONENTS_GEOMETRY_H_
#define SRC_COMPONENTS_GEOMETRY_H_
#include <SDL2pp/SDL2pp.hh>
struct Geometry {
  Geometry(const SDL2pp::Point& position,
       const SDL2pp::Point& direction,
       double angle = 0.0,
       int flip = 0)
      : position(position), direction(direction), angle(angle), flip(flip) {}

  SDL2pp::Point position;
  SDL2pp::Point direction;
  float angle = 0.0;
  int flip = 0;
};
#endif  // SRC_COMPONENTS_GEOMETRY_H_
