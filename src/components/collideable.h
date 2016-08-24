/* Copyright 2016 Tony Narlock. All rights reserved. */
#ifndef SRC_COMPONENTS_COLLIDEABLE_H_
#define SRC_COMPONENTS_COLLIDEABLE_H_
struct Collideable {
  explicit Collideable(float radius) : radius(radius) {}

  float radius;
};
#endif  // SRC_COMPONENTS_COLLIDEABLE_H_
