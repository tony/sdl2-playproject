/* Copyright 2016 Tony Narlock. All rights reserved. */
#include "bullet.h"
#include "config.h"

Bullet::Bullet(const std::unique_ptr<SDL2pp::Renderer>& renderer,
               const std::unique_ptr<ResourceManager>& resource_manager,
               SDL2pp::Point p,
               SDL2pp::Point v)
    : Actor(renderer,
            resource_manager,
            resource_manager->GetTexture("bullets1"),
            resource_manager->GetTexture("bullets1_tinted"),
            SDL2pp::Rect{12, 142, 3, 3},
            std::move(v),
            std::move(p)),
      stats(std::make_shared<BulletStats>()) {
  position.y += 12;
  position.x += 30;
}

bool Bullet::InBounds() {
  return MAIN_VIEWPORT_RECT.Contains(position);
}

void Bullet::Update() {
  position.x += velocity.x;
  position.y += velocity.y;
  if (InBounds()) {
    auto shadow_dimensions = subsprite_rect;
    auto shadow_position = position;
    shadow_position.x += 1;
    shadow_position.y += 1;

    renderer->Copy(*shadow, shadow_dimensions, shadow_position);

    renderer->Copy(*sprite, subsprite_rect,
                   SDL2pp::Rect(position, SDL2pp::Point{9, 9}));
  }
}
