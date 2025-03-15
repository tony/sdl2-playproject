/* Copyright 2016 Tony Narlock. All rights reserved. */
#include "bullet.h"

#include "config.h"
#include "stage.h"

Bullet::Bullet(const std::shared_ptr<LevelStage> &stage,
               const std::shared_ptr<Actor> &parent,
               const std::map<std::string, std::string> &string_map)
    : Actor(string2texture_map(string_map, stage->resource_manager),
            {0, 0},
            {9, 0},
            0,
            3),
      stats(std::make_shared<BulletStats>()),
      stage(stage)
{
  position = SDL2pp::Point{
      parent->position.x + parent->GetSize().x,
      (parent->position.y + (parent->GetSize().y / 2)) - GetSize().y};
}

void Bullet::Update(const std::unique_ptr<SDL2pp::Renderer> &renderer)
{
  position += velocity;

  Uint32 now = SDL_GetTicks();
  if (now - last_scale >= 50 && scale < 5)
  {
    scale++;
    last_scale = now;
  }
  tilt += 15;
  if (InBounds())
  {
    renderer->Copy(*sprites.at("default"), SDL2pp::NullOpt,
                   SDL2pp::Rect(position, GetSize()), tilt);
  }
}

// C++23 feature: Template compile-time bullet behavior
template <bool UseTrail, bool UseRotation>
void ApplyBulletEffects(Bullet &bullet, const std::unique_ptr<SDL2pp::Renderer> &renderer)
{
  // Base update logic
  bullet.position.x += bullet.velocity.x;
  bullet.position.y += bullet.velocity.y;

  // Conditional compile-time behavior based on template parameters
  if constexpr (UseTrail)
  {
    // Code for bullet trail effect (only compiled if UseTrail is true)
    auto trail_position = bullet.position;
    trail_position.x -= bullet.velocity.x * 2;
    // Render trail effect
    renderer->SetDrawColor(SDL2pp::Color(255, 255, 255, 128));
    renderer->DrawLine(bullet.position, trail_position);
  }

  if constexpr (UseRotation)
  {
    // Code for bullet rotation effect (only compiled if UseRotation is true)
    static double rotation = 0.0;
    rotation += 0.1;
    // Apply rotation effect (simplified example)
    renderer->SetDrawColor(SDL2pp::Color(255, 200, 200, 200));
    renderer->DrawPoint(bullet.position + SDL2pp::Point(5, 5));
  }
}
