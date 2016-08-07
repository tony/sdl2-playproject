#include "bullet.h"
#include "config.h"
#include "util.h"

Bullet::Bullet(SDL2pp::Renderer& renderer, SDL2pp::Rect p, SDL2pp::Point v)
    : Actor{renderer, std::move(p), std::move(v),
            "resources/gfx/m484BulletCollection1.png"} {
  sprite = LoadImageAlpha(renderer, "resources/gfx/m484BulletCollection1.png",
                          0, 0, 0);
  position.y += 12;
  position.x += 30;
  position.h = 9;
  position.w = 9;
};

bool Bullet::InBounds() {
  return MAIN_VIEWPORT_RECT.Contains(position);
}

void Bullet::HandleInput(const Uint8* currentKeyStates) {
  std::ignore = currentKeyStates;
  position.x += velocity.x;
  position.y += velocity.y;
  if (InBounds()) {
    renderer.Copy(sprite, SDL2pp::Rect{12, 142, 3, 3}, position);
  }
}
