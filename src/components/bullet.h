/* Copyright 2016 Tony Narlock. All rights reserved. */
#ifndef SRC_COMPONENTS_BULLET_H_
#define SRC_COMPONENTS_BULLET_H_
struct Bullet {
  explicit Bullet(int speed = 1) : speed(speed) {}

  unsigned int speed;
};
#endif  // SRC_COMPONENTS_BULLET_H_
