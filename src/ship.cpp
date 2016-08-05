#include "game.h"

const int SHIP_SPRITE_W = 33;
const int SHIP_SPRITE_H = 33;

SDL2pp::Texture LoadImageAlpha(SDL2pp::Renderer& renderer,
                               std::string spritePath,
                               Uint8 r,
                               Uint8 g,
                               Uint8 b) {
  auto surface = SDL2pp::Surface(spritePath);
  surface.SetColorKey(true, SDL_MapRGB(&surface.Lock().GetFormat(), r, g, b));
  auto image = SDL2pp::Texture(renderer, surface);
  return image;
}

Ship::Ship(SDL2pp::Renderer& renderer,
           SDL2pp::Rect position,
           SDL2pp::Point velocity)
    : Actor(renderer, position, velocity, "resources/gfx/modular_ships.png") {
  sprite =
      LoadImageAlpha(renderer, "resources/gfx/modular_ships.png", 13, 107, 178);
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
  renderer.Copy(sprite, subsprite[state], position);
}

void Ship::HandleEvents(const Uint8* currentKeyStates) {
  if (currentKeyStates[SDL_SCANCODE_UP] != 0 ||
      currentKeyStates[SDL_SCANCODE_W] != 0 ||
      currentKeyStates[SDL_SCANCODE_K] != 0) {
    state = SHIP_STATE_WALK_UP;
    position.y =
        CLAMP(position.y - static_cast<int>(MAIN_VIEWPORT_RECT.h * 0.01), 0,
              MAIN_VIEWPORT_RECT.h - SHIP_SPRITE_H);
  }

  if (currentKeyStates[SDL_SCANCODE_DOWN] != 0 ||
      currentKeyStates[SDL_SCANCODE_S] != 0 ||
      currentKeyStates[SDL_SCANCODE_J] != 0) {
    state = SHIP_STATE_WALK_DOWN;
    position.y =
        CLAMP(position.y + static_cast<int>(MAIN_VIEWPORT_RECT.h * 0.01), 0,
              MAIN_VIEWPORT_RECT.h - SHIP_SPRITE_H);
  }

  if (currentKeyStates[SDL_SCANCODE_LEFT] != 0 ||
      currentKeyStates[SDL_SCANCODE_A] != 0 ||
      currentKeyStates[SDL_SCANCODE_H] != 0) {
    state = SHIP_STATE_WALK_LEFT;
    position.x = CLAMP(position.x - static_cast<int>(SCREEN_WIDTH * 0.01), 0,
                       SCREEN_WIDTH - SHIP_SPRITE_W);
  }

  if (currentKeyStates[SDL_SCANCODE_RIGHT] != 0 ||
      currentKeyStates[SDL_SCANCODE_D] != 0 ||
      currentKeyStates[SDL_SCANCODE_L] != 0) {
    state = SHIP_STATE_WALK_RIGHT;
    position.x = CLAMP(position.x + static_cast<int>(SCREEN_WIDTH * 0.01), 0,
                       SCREEN_WIDTH - SHIP_SPRITE_W);
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
  for (auto& bullet : bullets) {
    bullet->HandleEvents(currentKeyStates);
  }
}

void Ship::SpawnBullet() {
  if (bullets.size() < SHIP_MAX_BULLETS) {
    bullets.push_back(new Bullet(renderer, position, velocity));
  }
}

Bullet::Bullet(SDL2pp::Renderer& renderer, SDL2pp::Rect p, SDL2pp::Point v)
    : Actor{renderer, std::move(p), std::move(v),
            "resources/gfx/m484BulletCollection1.png"} {
  sprite = LoadImageAlpha(renderer, "resources/gfx/m484BulletCollection1.png",
                          0, 0, 0);
  position.y += 12;
  position.x += 30;
};

bool Bullet::InBounds() {
  return MAIN_VIEWPORT_RECT.Contains(position);
}

void Bullet::HandleEvents(const Uint8* currentKeyStates) {
  std::ignore = currentKeyStates;
  position.x += velocity.x;
  position.y += velocity.y;
  position.h = 9;
  position.w = 9;
  if (InBounds()) {
    renderer.Copy(sprite, SDL2pp::Rect{12, 142, 3, 3}, position);
  }
}
