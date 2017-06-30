/* Copyright 2016 Tony Narlock. All rights reserved. */
#include "bullet.h"
#include "config.h"
#include "stage.h"

Bullet::Bullet(const std::shared_ptr<LevelStage>& stage,
               const std::shared_ptr<Actor>& parent,
               std::string texture_key,
               SDL2pp::Point p,
               SDL2pp::Point v)
    : Actor(string2texture_map({{"default", "bullet1"}}, stage->resource_manager),
            texture_key,
            std::move(p),
            std::move(v)),
      stats(std::make_shared<BulletStats>()),
      stage(stage) {
  position = SDL2pp::Point{
      parent->position.x + parent->GetSize().x,
      (parent->position.y + (parent->GetSize().y / 2)) - GetSize().y};
  scale = 3;
}

void Bullet::Update(const std::unique_ptr<SDL2pp::Renderer>& renderer) {
  position += velocity;

  Uint32 now = SDL_GetTicks();
  if (now - last_scale >= 50 && scale < 5) {
    scale++;
    last_scale = now;
  }
  tilt += 15;
  if (InBounds()) {
    renderer->Copy(
        *sprites.at("default"), SDL2pp::NullOpt,
        SDL2pp::Rect(position, (sprites.at("default")->GetSize() * scale)),
        tilt);
  }
}
