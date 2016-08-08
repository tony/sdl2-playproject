#include "bullet.h"
#include "config.h"
#include "resource.h"
#include "util.h"

Bullet::Bullet(SDL2pp::Renderer& renderer,
               const std::shared_ptr<ResourceManager>& resource_manager,
               SDL2pp::Rect p,
               SDL2pp::Point v)
    : Actor{renderer, resource_manager, std::move(p), std::move(v),
            "resources/gfx/m484BulletCollection1.png"} {
  sprite = LoadImageAlpha(renderer, "resources/gfx/m484BulletCollection1.png",
                          0, 0, 0);
  shadow = LoadImageShadow(renderer, "resources/gfx/m484BulletCollection1.png",
                           0, 0, 0);
  position.y += 12;
  position.x += 30;
  position.h = 9;
  position.w = 9;
};

bool Bullet::InBounds() {
  return MAIN_VIEWPORT_RECT.Contains(position);
}

void Bullet::Update() {
  position.x += velocity.x;
  position.y += velocity.y;
  if (InBounds()) {
    auto shadow_dimensions = SDL2pp::Rect{12, 142, 3, 3};
    auto shadow_position = position;
    shadow_position.x += 1;
    shadow_position.y += 1;

    renderer.Copy(shadow, shadow_dimensions, shadow_position);

    renderer.Copy(sprite, SDL2pp::Rect{12, 142, 3, 3}, position);
  }
}
