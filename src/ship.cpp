#include "config.h"
#include "ship.h"
#include "util.h"

Ship::Ship(SDL2pp::Renderer& renderer,
           SDL2pp::Rect position,
           SDL2pp::Point velocity)
    : Actor(renderer, position, velocity, "resources/gfx/modular_ships.png"),
      stats(std::make_shared<ShipStats>()) {
  sprite =
      LoadImageAlpha(renderer, "resources/gfx/modular_ships.png", 13, 107, 178);
  shadow = LoadImageShadow(renderer, "resources/gfx/modular_ships.png", 13, 107,
                           178);
  subsprite[SHIP_STATE_DEFAULT] =
      SDL2pp::Rect(126, 79, SHIP_SPRITE_W, SHIP_SPRITE_H);
  subsprite[SHIP_STATE_WALK_UP] =
      SDL2pp::Rect(126, 79, SHIP_SPRITE_W, SHIP_SPRITE_H);
  subsprite[SHIP_STATE_WALK_DOWN] =
      SDL2pp::Rect(126, 79, SHIP_SPRITE_W, SHIP_SPRITE_H);
  subsprite[SHIP_STATE_WALK_LEFT] =
      SDL2pp::Rect(126, 79, SHIP_SPRITE_W, SHIP_SPRITE_H);
  subsprite[SHIP_STATE_WALK_RIGHT] =
      SDL2pp::Rect(126, 79, SHIP_SPRITE_W, SHIP_SPRITE_H);
}

void Ship::Update() {
  auto shadow_dimensions = subsprite[state];
  auto shadow_position = position;
  shadow_position.x += 1;
  shadow_position.y += 1;

  renderer.Copy(shadow, shadow_dimensions, shadow_position);

  renderer.Copy(sprite, subsprite[state], position);

  for (auto& bullet : bullets) {
    bullet->Update();
  }
}

void Ship::HandleInput(const Uint8* currentKeyStates) {
  if (currentKeyStates[SDL_SCANCODE_UP] != 0 ||
      currentKeyStates[SDL_SCANCODE_W] != 0 ||
      currentKeyStates[SDL_SCANCODE_K] != 0) {
    state = SHIP_STATE_WALK_UP;
    position.y =
        clamp(position.y - static_cast<int>(MAIN_VIEWPORT_RECT.h * 0.01), 0,
              MAIN_VIEWPORT_RECT.h - SHIP_SPRITE_H);
  }

  if (currentKeyStates[SDL_SCANCODE_DOWN] != 0 ||
      currentKeyStates[SDL_SCANCODE_S] != 0 ||
      currentKeyStates[SDL_SCANCODE_J] != 0) {
    state = SHIP_STATE_WALK_DOWN;
    position.y =
        clamp(position.y + static_cast<int>(MAIN_VIEWPORT_RECT.h * 0.01), 0,
              MAIN_VIEWPORT_RECT.h - SHIP_SPRITE_H);
  }

  if (currentKeyStates[SDL_SCANCODE_LEFT] != 0 ||
      currentKeyStates[SDL_SCANCODE_A] != 0 ||
      currentKeyStates[SDL_SCANCODE_H] != 0) {
    state = SHIP_STATE_WALK_LEFT;
    position.x = clamp(position.x - static_cast<int>(SCREEN_RECT.w * 0.01), 0,
                       SCREEN_RECT.w - SHIP_SPRITE_W);
  }

  if (currentKeyStates[SDL_SCANCODE_RIGHT] != 0 ||
      currentKeyStates[SDL_SCANCODE_D] != 0 ||
      currentKeyStates[SDL_SCANCODE_L] != 0) {
    state = SHIP_STATE_WALK_RIGHT;
    position.x = clamp(position.x + static_cast<int>(SCREEN_RECT.w * 0.01), 0,
                       SCREEN_RECT.w - SHIP_SPRITE_W);
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
    bullets.push_back(new Bullet(renderer, position, velocity));
  }
}
