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
  if (currentKeyStates[SDL_SCANCODE_UP] != 0 ||
      currentKeyStates[SDL_SCANCODE_W] != 0 ||
      currentKeyStates[SDL_SCANCODE_K] != 0) {
    state = EnemyState::UP;
    position.y =
        clamp(position.y - static_cast<int>(MAIN_VIEWPORT_RECT.h * 0.01), 0,
              MAIN_VIEWPORT_RECT.h - subsprite_rect.h);
  }

  if (currentKeyStates[SDL_SCANCODE_DOWN] != 0 ||
      currentKeyStates[SDL_SCANCODE_S] != 0 ||
      currentKeyStates[SDL_SCANCODE_J] != 0) {
    state = EnemyState::DOWN;
    position.y =
        clamp(position.y + static_cast<int>(MAIN_VIEWPORT_RECT.h * 0.01), 0,
              MAIN_VIEWPORT_RECT.h - subsprite_rect.h);
  }

  if (currentKeyStates[SDL_SCANCODE_LEFT] != 0 ||
      currentKeyStates[SDL_SCANCODE_A] != 0 ||
      currentKeyStates[SDL_SCANCODE_H] != 0) {
    state = EnemyState::LEFT;
    position.x = clamp(position.x - static_cast<int>(SCREEN_RECT.w * 0.01), 0,
                       SCREEN_RECT.w - subsprite_rect.w);
  }

  if (currentKeyStates[SDL_SCANCODE_RIGHT] != 0 ||
      currentKeyStates[SDL_SCANCODE_D] != 0 ||
      currentKeyStates[SDL_SCANCODE_L] != 0) {
    state = EnemyState::RIGHT;
    position.x = clamp(position.x + static_cast<int>(SCREEN_RECT.w * 0.01), 0,
                       SCREEN_RECT.w - subsprite_rect.w);
  }

  if (*(currentKeyStates + SDL_SCANCODE_SPACE) != 0) {
    Uint32 now = SDL_GetTicks();
    if (now - last_shot >= shooting_delay) {
      SpawnBullet();
      last_shot = now;
    }
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
