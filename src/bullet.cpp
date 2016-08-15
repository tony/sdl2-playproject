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
  LoadResources();
}

void Bullet::LoadResources() {
  if (!resource_manager->HasTexture("bullet1")) {
    static auto target1 = std::make_shared<SDL2pp::Texture>(
        *renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET,
        GetSubspriteRect().w, GetSubspriteRect().h);

    target1->SetBlendMode(SDL_BLENDMODE_BLEND);
    renderer->SetTarget(*target1);
    renderer->Clear();
    renderer->SetDrawBlendMode(SDL_BLENDMODE_BLEND);

    renderer->Copy(*shadow_sheet, GetSubspriteRect() + SDL2pp::Point{1, 1},
                   SDL2pp::NullOpt);
    renderer->Copy(*sprite_sheet, GetSubspriteRect(), SDL2pp::NullOpt);

    renderer->SetTarget();
    resource_manager->AddTexture("bullet1", target1);
  }
}

bool Bullet::InBounds() {
  return MAIN_VIEWPORT_RECT.Contains(position);
}

void Bullet::Update() {
  position += velocity;
  if (InBounds()) {
    renderer->Copy(
        *resource_manager->GetTexture("bullet1"),
        SDL2pp::Rect{0, 0, GetSubspriteRect().w * 3, GetSubspriteRect().h * 3},
        position);
  }
}
