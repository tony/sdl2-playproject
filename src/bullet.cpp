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
            std::move(p),
            std::move(v)),
      stats(std::make_shared<BulletStats>()) {
  position.y -= GetSize().y;
  scale = 3;
}

bool Bullet::InBounds() {
  return MAIN_VIEWPORT_RECT.Contains(position);
}

void Bullet::Update() {
  position += velocity;

  Uint32 now = SDL_GetTicks();
  if (now - last_scale >= 50 && scale < 5) {
    scale++;
    last_scale = now;
  }
  angle += 15;
  if (InBounds()) {
    renderer->Copy(
        *resource_manager->GetTexture("bullet1"), SDL2pp::NullOpt,
        SDL2pp::Rect(
            position,
            (resource_manager->GetTexture("bullet1")->GetSize() * scale)),
        angle);
  }
}
