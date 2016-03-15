#include "game.h"
#include <iostream>

extern const double SCREEN_HEIGHT;
extern const double SCREEN_WIDTH;
extern const SDL2pp::Rect BOTTOM_VIEWPORT_RECT;

const int SHOOTING_DELAY = 200;
const int HERO_SPRITE_W = 30;
const int HERO_SPRITE_H = 30;

Hero::Hero(SDL2pp::Renderer& renderer) :
  spriteSheet(renderer, get_full_path("resources/elliot/spritesheet.png")),
  boomerangSprite(renderer, get_full_path("resources/boomerang.png")),
  renderer(renderer)
{
  HeroState[HERO_STATE_DEFAULT] = SDL2pp::Rect(0, 0, HERO_SPRITE_W, HERO_SPRITE_H);
  HeroState[HERO_STATE_WALK_UP] = SDL2pp::Rect(0, 1010, HERO_SPRITE_W, HERO_SPRITE_H);
  HeroState[HERO_STATE_WALK_DOWN] = SDL2pp::Rect(0, 0, HERO_SPRITE_W, HERO_SPRITE_H);
  HeroState[HERO_STATE_WALK_LEFT] = SDL2pp::Rect(0, 505, HERO_SPRITE_W, HERO_SPRITE_H);
  HeroState[HERO_STATE_WALK_RIGHT] = SDL2pp::Rect(0, 720, HERO_SPRITE_W, HERO_SPRITE_H);
}

void Hero::loop(const Uint8* currentKeyStates) {
  if (currentKeyStates[SDL_SCANCODE_UP] || currentKeyStates[SDL_SCANCODE_W] ||
      currentKeyStates[SDL_SCANCODE_K]) {
    state = HERO_STATE_WALK_UP;
    position.y = CLAMP(position.y - MAIN_VIEWPORT_RECT.h * 0.01, 0,
        MAIN_VIEWPORT_RECT.h - HERO_SPRITE_H);
  }

  if (currentKeyStates[SDL_SCANCODE_DOWN] || currentKeyStates[SDL_SCANCODE_S] ||
      currentKeyStates[SDL_SCANCODE_J]) {
    state = HERO_STATE_WALK_DOWN;
    position.y = CLAMP(position.y + MAIN_VIEWPORT_RECT.h * 0.01, 0,
        MAIN_VIEWPORT_RECT.h - HERO_SPRITE_H);
  }

  if (currentKeyStates[SDL_SCANCODE_LEFT] || currentKeyStates[SDL_SCANCODE_A] ||
      currentKeyStates[SDL_SCANCODE_H]) {
    state = HERO_STATE_WALK_LEFT;
    position.x = CLAMP(position.x - SCREEN_WIDTH * 0.01, 0,
        SCREEN_WIDTH - HERO_SPRITE_W);
  }

  if (currentKeyStates[SDL_SCANCODE_RIGHT] ||
      currentKeyStates[SDL_SCANCODE_D] || currentKeyStates[SDL_SCANCODE_L]) {
    state = HERO_STATE_WALK_RIGHT;
    position.x = CLAMP(position.x + SCREEN_WIDTH * 0.01, 0,
        SCREEN_WIDTH - HERO_SPRITE_W);
  }

  if (currentKeyStates[SDL_SCANCODE_SPACE]) {
    Uint32 now = SDL_GetTicks();
    if (now - last_shot >= SHOOTING_DELAY) {
      CreateBoomerang();
      last_shot = now;
    }
  }

  // boomerang drawing and clean up
  auto it = boomerangs.begin();
  while (it != boomerangs.end()) {
    if ((**it).outOfBounds()) it = boomerangs.erase(it);
    else ++it;
  }
  for (auto& boomerang : boomerangs) {
    boomerang->loop();
  }

}

void Hero::CreateBoomerang(void) {
  if (boomerangs.size() < HERO_MAX_BOOMERANGS) {
    SDL2pp::Point velocity;

    switch (state) {
      case HERO_STATE_WALK_UP:
        velocity.x = 0;
        velocity.y = -1;
        break;
      case HERO_STATE_WALK_DOWN:
        velocity.x = 0;
        velocity.y = 1;
        break;
      case HERO_STATE_WALK_LEFT:
        velocity.x = -1;
        velocity.y = 0;
        break;
      case HERO_STATE_WALK_RIGHT:
        velocity.x = 1;
        velocity.y = 0;
        break;
      default:
        velocity.x = 0;
        velocity.y = 0;
        break;
    }
    boomerangs.push_back(new Boomerang(this, renderer, position, velocity));
  }
}

Boomerang::Boomerang(Hero* hero, SDL2pp::Renderer& renderer, SDL2pp::Rect p, SDL2pp::Point v) :
  renderer(renderer),
  position(p),
  velocity(v),
  hero(hero)
{}

bool Boomerang::outOfBounds() {
  return (position.x > MAIN_VIEWPORT_RECT.w ||
      position.w > MAIN_VIEWPORT_RECT.w || position.x < 0 ||
      position.y > MAIN_VIEWPORT_RECT.h ||
      position.h > MAIN_VIEWPORT_RECT.h || position.y < 0);
}

void Boomerang::loop() {
  position.x += velocity.x;
  position.y += velocity.y;
  if (!outOfBounds()) {
    renderer.Copy(hero->boomerangSprite, SDL2pp::NullOpt, position);
  }
}
