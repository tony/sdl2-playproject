#include "game.h"

const int SHOOTING_DELAY = 200;
const int HERO_SPRITE_W = 33;
const int HERO_SPRITE_H = 33;

SDL2pp::Texture loadImageAlpha(SDL2pp::Renderer& renderer,
                               std::string spritePath,
                               Uint8 r,
                               Uint8 g,
                               Uint8 b) {
  auto surface = SDL2pp::Surface(spritePath);
  surface.SetColorKey(true, SDL_MapRGB(&surface.Lock().GetFormat(), r, g, b));
  auto image = SDL2pp::Texture(renderer, surface);
  return image;
}

Hero::Hero(SDL2pp::Renderer& renderer,
           SDL2pp::Rect position,
           SDL2pp::Point velocity)
    : Actor(renderer, position, velocity, "resources/gfx/modular_ships.png") {
  sprite =
      loadImageAlpha(renderer, "resources/gfx/modular_ships.png", 13, 107, 178);
  subsprite[HERO_STATE_DEFAULT] =
      SDL2pp::Rect(126, 79, HERO_SPRITE_W, HERO_SPRITE_H);
  subsprite[HERO_STATE_WALK_UP] =
      SDL2pp::Rect(126, 79, HERO_SPRITE_W, HERO_SPRITE_H);
  subsprite[HERO_STATE_WALK_DOWN] =
      SDL2pp::Rect(126, 79, HERO_SPRITE_W, HERO_SPRITE_H);
  subsprite[HERO_STATE_WALK_LEFT] =
      SDL2pp::Rect(126, 79, HERO_SPRITE_W, HERO_SPRITE_H);
  subsprite[HERO_STATE_WALK_RIGHT] =
      SDL2pp::Rect(126, 79, HERO_SPRITE_W, HERO_SPRITE_H);
}

void Hero::update() {
  renderer.Copy(sprite, subsprite[state], position);
}

void Hero::handleEvents(const Uint8* currentKeyStates) {
  if (currentKeyStates[SDL_SCANCODE_UP] != 0 ||
      currentKeyStates[SDL_SCANCODE_W] != 0 ||
      currentKeyStates[SDL_SCANCODE_K] != 0) {
    state = HERO_STATE_WALK_UP;
    position.y =
        CLAMP(position.y - static_cast<int>(MAIN_VIEWPORT_RECT.h * 0.01), 0,
              MAIN_VIEWPORT_RECT.h - HERO_SPRITE_H);
  }

  if (currentKeyStates[SDL_SCANCODE_DOWN] != 0 ||
      currentKeyStates[SDL_SCANCODE_S] != 0 ||
      currentKeyStates[SDL_SCANCODE_J] != 0) {
    state = HERO_STATE_WALK_DOWN;
    position.y =
        CLAMP(position.y + static_cast<int>(MAIN_VIEWPORT_RECT.h * 0.01), 0,
              MAIN_VIEWPORT_RECT.h - HERO_SPRITE_H);
  }

  if (currentKeyStates[SDL_SCANCODE_LEFT] != 0 ||
      currentKeyStates[SDL_SCANCODE_A] != 0 ||
      currentKeyStates[SDL_SCANCODE_H] != 0) {
    state = HERO_STATE_WALK_LEFT;
    position.x = CLAMP(position.x - static_cast<int>(SCREEN_WIDTH * 0.01), 0,
                       SCREEN_WIDTH - HERO_SPRITE_W);
  }

  if (currentKeyStates[SDL_SCANCODE_RIGHT] != 0 ||
      currentKeyStates[SDL_SCANCODE_D] != 0 ||
      currentKeyStates[SDL_SCANCODE_L] != 0) {
    state = HERO_STATE_WALK_RIGHT;
    position.x = CLAMP(position.x + static_cast<int>(SCREEN_WIDTH * 0.01), 0,
                       SCREEN_WIDTH - HERO_SPRITE_W);
  }

  if (*(currentKeyStates + SDL_SCANCODE_SPACE) != 0) {
    Uint32 now = SDL_GetTicks();
    if (now - last_shot >= SHOOTING_DELAY) {
      createBullet();
      last_shot = now;
    }
  }

  // boomerang drawing and clean up
  auto it = boomerangs.begin();
  while (it != boomerangs.end()) {
    if (!(**it).inBounds()) {
      it = boomerangs.erase(it);
    } else {
      ++it;
    }
  }
  for (auto& boomerang : boomerangs) {
    boomerang->handleEvents(currentKeyStates);
  }
}

void Hero::createBullet() {
  if (boomerangs.size() < HERO_MAX_BOOMERANGS) {
    SDL2pp::Point velocity;
    velocity.x = 2;
    velocity.y = 0;
    boomerangs.push_back(new Bullet(renderer, position, velocity));
  }
}

Bullet::Bullet(SDL2pp::Renderer& renderer, SDL2pp::Rect p, SDL2pp::Point v)
    : Actor{renderer, std::move(p), std::move(v),
            "resources/gfx/m484BulletCollection1.png"} {
  sprite = loadImageAlpha(renderer, "resources/gfx/m484BulletCollection1.png",
                          0, 0, 0);
  position.y += 12;
  position.x += 30;
};

bool Bullet::inBounds() {
  return MAIN_VIEWPORT_RECT.Contains(position);
}

void Bullet::handleEvents(const Uint8* currentKeyStates) {
  std::ignore = currentKeyStates;
  position.x += velocity.x;
  position.y += velocity.y;
  position.h = 9;
  position.w = 9;
  if (inBounds()) {
    renderer.Copy(sprite, SDL2pp::Rect{12, 103, 3, 3}, position);
  }
}
