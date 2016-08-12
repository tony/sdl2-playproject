#include "config.h"
#include "resource.h"
#include "enemy.h"
#include "util.h"

Enemy::Enemy(const std::unique_ptr<SDL2pp::Renderer>& renderer,
             const std::unique_ptr<ResourceManager>& resource_manager,
             SDL2pp::Point position,
             SDL2pp::Point velocity)
    : Actor(renderer,
            resource_manager,
            position,
            velocity,
            SDL2pp::Rect{126, 79, 33, 33},
            resource_manager->GetTexture("modular_ships"),
            resource_manager->GetTexture("modular_ships_tinted")),
      stats(std::make_shared<EnemyStats>()) {
  subsprites[static_cast<int>(EnemyState::DEFAULT)] = subsprite_rect;
  subsprites[static_cast<int>(EnemyState::UP)] = subsprite_rect;
  subsprites[static_cast<int>(EnemyState::DOWN)] = subsprite_rect;
  subsprites[static_cast<int>(EnemyState::LEFT)] = subsprite_rect;
  subsprites[static_cast<int>(EnemyState::RIGHT)] = subsprite_rect;
}

void Enemy::Update() {
  position.x--;
  // position.x =
  //     clamp(position.x - static_cast<int>(MAIN_VIEWPORT_RECT.w * 0.01), 0,
  //           MAIN_VIEWPORT_RECT.w - subsprite_rect.w);

  auto shadow_dimensions = subsprites[static_cast<int>(state)];
  auto shadow_position = position;
  shadow_position.x += 1;
  shadow_position.y += 1;

  renderer->Copy(*shadow, shadow_dimensions, shadow_position);

  renderer->Copy(*sprite, subsprites[static_cast<int>(state)], position);

  for (auto& bullet : bullets) {
    bullet->Update();
  }
}

void Enemy::HandleInput(const Uint8* currentKeyStates) {
  std::ignore = currentKeyStates;
  Uint32 now = SDL_GetTicks();
  if (now - last_shot >= shooting_delay) {
    SpawnBullet();
    last_shot = now;
  }

  // bullet drawing and clean up
  bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                               [](auto& b) { return !b->InBounds(); }),
                bullets.end());
}

void Enemy::SpawnBullet() {
  if (bullets.size() < SHIP_MAX_BULLETS) {
    bullets.push_back(std::make_shared<Bullet>(renderer, resource_manager,
                                               position, velocity));
  }
}
