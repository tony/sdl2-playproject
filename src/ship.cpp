/* Copyright 2016 Tony Narlock. All rights reserved. */
#include "config.h"
#include "resource.h"
#include "ship.h"
#include "util.h"

Player::Player(const std::unique_ptr<SDL2pp::Renderer>& renderer,
               const std::unique_ptr<ResourceManager>& resource_manager,
               const std::shared_ptr<spdlog::logger>& console)
    : ship(std::make_unique<Ship>(renderer,
                                  resource_manager,
                                  console,
                                  "ship1",
                                  SDL2pp::Point{30, 30},
                                  SDL2pp::Point{0, 0})) {}

void Player::HandleInput(const Uint8* currentKeyStates) {
  ship->HandleInput(currentKeyStates);
}

void Ship::HandleInput(const Uint8* currentKeyStates) {
  if (currentKeyStates[SDL_SCANCODE_UP] | currentKeyStates[SDL_SCANCODE_W] |
      currentKeyStates[SDL_SCANCODE_K]) {
    state = ActorState::UP;
    position.y =
        clamp(position.y - static_cast<int>(MAIN_VIEWPORT_RECT.h * 0.01), 0,
              MAIN_VIEWPORT_RECT.h - GetSprite()->GetHeight());
  }

  if (currentKeyStates[SDL_SCANCODE_DOWN] | currentKeyStates[SDL_SCANCODE_S] |
      currentKeyStates[SDL_SCANCODE_J]) {
    state = ActorState::DOWN;
    position.y =
        clamp(position.y + static_cast<int>(MAIN_VIEWPORT_RECT.h * 0.01), 0,
              MAIN_VIEWPORT_RECT.h - GetSprite()->GetHeight());
  }

  if (currentKeyStates[SDL_SCANCODE_LEFT] | currentKeyStates[SDL_SCANCODE_A] |
      currentKeyStates[SDL_SCANCODE_H]) {
    state = ActorState::LEFT;
    position.x = clamp(position.x - static_cast<int>(SCREEN_RECT.w * 0.01), 0,
                       SCREEN_RECT.w - GetSprite()->GetWidth());
  }

  if (currentKeyStates[SDL_SCANCODE_RIGHT] | currentKeyStates[SDL_SCANCODE_D] |
      currentKeyStates[SDL_SCANCODE_L]) {
    state = ActorState::RIGHT;
    position.x = clamp(position.x + static_cast<int>(SCREEN_RECT.w * 0.01), 0,
                       SCREEN_RECT.w - GetSprite()->GetWidth());
  }

  if (*(currentKeyStates + SDL_SCANCODE_SPACE) != 0) {
    Uint32 now = SDL_GetTicks();
    if (now - last_shot >= shooting_delay) {
      SpawnBullet();
      last_shot = now;
    }
  }
}

Ship::Ship(const std::unique_ptr<SDL2pp::Renderer>& renderer,
           const std::unique_ptr<ResourceManager>& resource_manager,
           const std::shared_ptr<spdlog::logger>& console,
           std::string texture_key,
           SDL2pp::Optional<SDL2pp::Point> position,
           SDL2pp::Point velocity,
           ShipStats stats,
           int flip)
    : Actor(renderer, resource_manager, texture_key, velocity, position),
      stats(std::make_shared<ShipStats>(stats)),
      console(console),
      flip(flip) {}

void Ship::Update() {
  if (hit) {
    renderer->Copy(
        *resource_manager->GetTexture("ship1_hit"),
        SDL2pp::Rect{0, 0, GetSubspriteRect().w, GetSubspriteRect().h},
        position, 0, SDL2pp::NullOpt, flip);
    Uint32 now = SDL_GetTicks();
    if (now - last_hit >= 100) {
      hit = false;
    }
  } else {
    renderer->Copy(
        *resource_manager->GetTexture(texture_key),
        SDL2pp::Rect{0, 0, GetSubspriteRect().w, GetSubspriteRect().h},
        position, 0, SDL2pp::NullOpt, flip);
  }

  // bullet drawing and clean up
  bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                               [](auto& b) { return !b->InBounds(); }),
                bullets.end());
  for (auto& bullet : bullets) {
    bullet->Update();
  }
}

void Ship::SpawnBullet() {
  if (bullets.size() < SHIP_MAX_BULLETS) {
    bullets.push_back(std::make_shared<Bullet>(renderer, resource_manager,
                                               "bullet1", position));
  }
}

void Ship::OnHitByBullet(std::shared_ptr<Bullet> bullet) {
  hit = true;
  last_hit = SDL_GetTicks();
  stats->health -= bullet->stats->damage;
  console->info("hit for {}, {}/{}", bullet->stats->damage, stats->health,
                stats->health_max);
}
