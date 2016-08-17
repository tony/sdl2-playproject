/* Copyright 2016 Tony Narlock. All rights reserved. */
#include "bullet.h"
#include "config.h"

Bullet::Bullet(const std::unique_ptr<SDL2pp::Renderer>& renderer,
               const std::unique_ptr<ResourceManager>& resource_manager,
               std::string texture_key,
               SDL2pp::Point p,
               SDL2pp::Point v)
    : Actor(renderer,
            resource_manager,
            texture_key,
            std::move(v),
            std::move(p)),
      stats(std::make_shared<BulletStats>()) {
  position.y += GetSize().y / 2;
  position.x += 30;
  scale = 9;
}

bool Bullet::InBounds() {
  return MAIN_VIEWPORT_RECT.Contains(position);
}

void Bullet::Update() {
  position += velocity;
  if (InBounds()) {
    renderer->Copy(
        *resource_manager->GetTexture("bullet1"), SDL2pp::NullOpt,
        SDL2pp::Rect(
            position,
            (resource_manager->GetTexture("bullet1")->GetSize() * scale)));
  }
}
