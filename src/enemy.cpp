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
  subsprites[static_cast<int>(ActorState::DEFAULT)] = subsprite_rect;
}

void Enemy::Update() {
  position.x--;

  auto shadow_position = SDL2pp::Point{position.x + 1, position.y + 1};

  renderer->Copy(*shadow, GetSubspriteRect(), shadow_position);
  if (hit) {
    renderer->Copy(*resource_manager->GetTexture("modular_ships_tinted_red"),
                   GetSubspriteRect(), position);
    Uint32 now = SDL_GetTicks();
    if (now - last_hit >= 100) {
      hit = false;
    }
  } else {
    renderer->Copy(*sprite, GetSubspriteRect(), position);
  }

  for (auto& bullet : bullets) {
    bullet->Update();
  }
}

void Enemy::OnHitByBullet(std::shared_ptr<Bullet> bullet) {
  std::ignore = bullet;

  hit = true;
  last_hit = SDL_GetTicks();
  stats->health -= bullet->stats->damage;
  console->info("hit for {}, {}/{}", bullet->stats->damage, stats->health,
                stats->health_max);
}

void Enemy::HandleInput(const Uint8* currentKeyStates) {
  std::ignore = currentKeyStates;
  Uint32 now = SDL_GetTicks();
  if (now - last_shot >= shooting_delay) {
    SpawnBullet();
    last_shot = now;
  }

  // sweep out of bounds bullets
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
