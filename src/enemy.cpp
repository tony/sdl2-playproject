#include "config.h"
#include "resource.h"
#include "enemy.h"
#include "util.h"

Enemy::Enemy(const std::unique_ptr<SDL2pp::Renderer>& renderer,
             const std::unique_ptr<ResourceManager>& resource_manager,
             const std::shared_ptr<spdlog::logger>& console,
             SDL2pp::Point position,
             SDL2pp::Point velocity)
    : Actor(renderer,
            resource_manager,
            position,
            velocity,
            SDL2pp::Rect{126, 79, 33, 33},
            resource_manager->GetTexture("modular_ships_tinted_tan"),
            resource_manager->GetTexture("modular_ships_tinted")),
      stats(std::make_shared<EnemyStats>()),
      console(console) {
  subsprites[static_cast<int>(EnemyState::DEFAULT)] = subsprite_rect;
  subsprites[static_cast<int>(EnemyState::UP)] = subsprite_rect;
  subsprites[static_cast<int>(EnemyState::DOWN)] = subsprite_rect;
  subsprites[static_cast<int>(EnemyState::LEFT)] = subsprite_rect;
  subsprites[static_cast<int>(EnemyState::RIGHT)] = subsprite_rect;
}

void Enemy::Update() {
  position.x--;

  auto shadow_position = SDL2pp::Point{position.x + 1, position.y + 1};

  renderer->Copy(*shadow, subsprites[static_cast<int>(state)], shadow_position);
  if (hit) {
    renderer->Copy(*resource_manager->GetTexture("modular_ships_tinted_red"),
                   subsprites[static_cast<int>(state)], position);
    Uint32 now = SDL_GetTicks();
    if (now - last_hit >= 100) {
      hit = false;
    }
  } else {
    renderer->Copy(*sprite, subsprites[static_cast<int>(state)], position);
  }

  for (auto& bullet : bullets) {
    bullet->Update();
  }
}

void Enemy::Strike(std::shared_ptr<Bullet> bullet) {
  std::ignore = bullet;
  console->info("hit!");
  hit = true;
  last_hit = SDL_GetTicks();
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
