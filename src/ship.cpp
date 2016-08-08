#include "config.h"
#include "resource.h"
#include "ship.h"
#include "util.h"

Ship::Ship(const std::unique_ptr<SDL2pp::Renderer>& renderer,
           const std::unique_ptr<ResourceManager>& resource_manager,
           SDL2pp::Rect position,
           SDL2pp::Point velocity)
    : Actor(renderer,
            resource_manager,
            position,
            velocity,
            SDL2pp::Rect{126, 79, 33, 33},
            resource_manager->GetTexture("modular_ships"),
            resource_manager->GetTexture("modular_ships_tinted")),
      stats(std::make_shared<ShipStats>()) {
  subsprites[ShipState::DEFAULT] = subsprite_rect;
  subsprites[ShipState::UP] = subsprite_rect;
  subsprites[ShipState::DOWN] = subsprite_rect;
  subsprites[ShipState::LEFT] = subsprite_rect;
  subsprites[ShipState::RIGHT] = subsprite_rect;
}

void Ship::Update() {
  auto shadow_dimensions = subsprites[state];
  auto shadow_position = position;
  shadow_position.x += 1;
  shadow_position.y += 1;

  renderer->Copy(*shadow, shadow_dimensions, shadow_position);

  renderer->Copy(*sprite, subsprites[state], position);

  for (auto& bullet : bullets) {
    bullet->Update();
  }
}

void Ship::HandleInput(const Uint8* currentKeyStates) {
  if (currentKeyStates[SDL_SCANCODE_UP] != 0 ||
      currentKeyStates[SDL_SCANCODE_W] != 0 ||
      currentKeyStates[SDL_SCANCODE_K] != 0) {
    state = ShipState::UP;
    position.y =
        clamp(position.y - static_cast<int>(MAIN_VIEWPORT_RECT.h * 0.01), 0,
              MAIN_VIEWPORT_RECT.h - subsprite_rect.h);
  }

  if (currentKeyStates[SDL_SCANCODE_DOWN] != 0 ||
      currentKeyStates[SDL_SCANCODE_S] != 0 ||
      currentKeyStates[SDL_SCANCODE_J] != 0) {
    state = ShipState::DOWN;
    position.y =
        clamp(position.y + static_cast<int>(MAIN_VIEWPORT_RECT.h * 0.01), 0,
              MAIN_VIEWPORT_RECT.h - subsprite_rect.h);
  }

  if (currentKeyStates[SDL_SCANCODE_LEFT] != 0 ||
      currentKeyStates[SDL_SCANCODE_A] != 0 ||
      currentKeyStates[SDL_SCANCODE_H] != 0) {
    state = ShipState::LEFT;
    position.x = clamp(position.x - static_cast<int>(SCREEN_RECT.w * 0.01), 0,
                       SCREEN_RECT.w - subsprite_rect.w);
  }

  if (currentKeyStates[SDL_SCANCODE_RIGHT] != 0 ||
      currentKeyStates[SDL_SCANCODE_D] != 0 ||
      currentKeyStates[SDL_SCANCODE_L] != 0) {
    state = ShipState::RIGHT;
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
  auto it = bullets.begin();
  while (it != bullets.end()) {
    if (!(**it).InBounds()) {
      it = bullets.erase(it);
    } else {
      ++it;
    }
  }
}

void Ship::SpawnBullet() {
  if (bullets.size() < SHIP_MAX_BULLETS) {
    bullets.push_back(
        new Bullet(renderer, resource_manager, position, velocity));
  }
}
