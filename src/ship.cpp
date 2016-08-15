/* Copyright 2016 Tony Narlock. All rights reserved. */
#include "config.h"
#include "resource.h"
#include "ship.h"
#include "util.h"

Player::Player(const std::unique_ptr<SDL2pp::Renderer>& renderer,
               const std::unique_ptr<ResourceManager>& resource_manager,
               const std::shared_ptr<spdlog::logger>& console)
    : ship(std::make_unique<Ship>(
          renderer,
          resource_manager,
          console,
          resource_manager->GetTextureSheet("modular_ships"),
          SDL2pp::Point{30, 30})) {}

void Player::HandleInput(const Uint8* currentKeyStates) {
  ship->HandleInput(currentKeyStates);
}

void Ship::HandleInput(const Uint8* currentKeyStates) {
  if (currentKeyStates[SDL_SCANCODE_UP] | currentKeyStates[SDL_SCANCODE_W] |
      currentKeyStates[SDL_SCANCODE_K]) {
    state = ActorState::UP;
    position.y =
        clamp(position.y - static_cast<int>(MAIN_VIEWPORT_RECT.h * 0.01), 0,
              MAIN_VIEWPORT_RECT.h - subsprite_rect.h);
  }

  if (currentKeyStates[SDL_SCANCODE_DOWN] | currentKeyStates[SDL_SCANCODE_S] |
      currentKeyStates[SDL_SCANCODE_J]) {
    state = ActorState::DOWN;
    position.y =
        clamp(position.y + static_cast<int>(MAIN_VIEWPORT_RECT.h * 0.01), 0,
              MAIN_VIEWPORT_RECT.h - subsprite_rect.h);
  }

  if (currentKeyStates[SDL_SCANCODE_LEFT] | currentKeyStates[SDL_SCANCODE_A] |
      currentKeyStates[SDL_SCANCODE_H]) {
    state = ActorState::LEFT;
    position.x = clamp(position.x - static_cast<int>(SCREEN_RECT.w * 0.01), 0,
                       SCREEN_RECT.w - subsprite_rect.w);
  }

  if (currentKeyStates[SDL_SCANCODE_RIGHT] | currentKeyStates[SDL_SCANCODE_D] |
      currentKeyStates[SDL_SCANCODE_L]) {
    state = ActorState::RIGHT;
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
}

Ship::Ship(const std::unique_ptr<SDL2pp::Renderer>& renderer,
           const std::unique_ptr<ResourceManager>& resource_manager,
           const std::shared_ptr<spdlog::logger>& console,
           const std::shared_ptr<SDL2pp::Texture>& sprite_sheet,
           SDL2pp::Optional<SDL2pp::Point> position,
           SDL2pp::Point velocity,
           ShipStats stats,
           int flip)
    : Actor(renderer,
            resource_manager,
            sprite_sheet,
            resource_manager->GetTextureSheet("modular_ships_tinted"),
            SDL2pp::Rect{126, 79, 33, 33},
            velocity,
            position),
      stats(std::make_shared<ShipStats>(stats)),
      console(console),
      flip(flip) {
  subsprites[static_cast<int>(ActorState::DEFAULT)] = subsprite_rect;
  subsprites[static_cast<int>(ActorState::UP)] = subsprite_rect;
  subsprites[static_cast<int>(ActorState::DOWN)] = subsprite_rect;
  subsprites[static_cast<int>(ActorState::LEFT)] = subsprite_rect;
  subsprites[static_cast<int>(ActorState::RIGHT)] = subsprite_rect;
  LoadResources();
}

void Ship::LoadResources() {
  if (!resource_manager->HasTexture("modular_ships_shadowed")) {
    static auto target1 = std::make_shared<SDL2pp::Texture>(
        *renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET,
        GetSubspriteRect().w, GetSubspriteRect().h);

    target1->SetBlendMode(SDL_BLENDMODE_BLEND);
    renderer->SetTarget(*target1);
    renderer->Clear();
    renderer->SetDrawBlendMode(SDL_BLENDMODE_BLEND);

    renderer->Copy(*shadow_sheet, GetSubspriteRect() + SDL2pp::Point{1, 1},
                   SDL2pp::NullOpt);
    renderer->Copy(*sprite_sheet, GetSubspriteRect(), SDL2pp::NullOpt);

    resource_manager->AddTexture("modular_ships_shadowed", target1);
    renderer->SetTarget();
  }
  if (!resource_manager->HasTexture("modular_ships_hit")) {
    static auto target2 = std::make_shared<SDL2pp::Texture>(
        *renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET,
        GetSubspriteRect().w, GetSubspriteRect().h);

    target2->SetBlendMode(SDL_BLENDMODE_BLEND);
    renderer->SetTarget(*target2);
    renderer->Clear();
    renderer->SetDrawBlendMode(SDL_BLENDMODE_BLEND);

    renderer->Copy(
        *resource_manager->GetTextureSheet("modular_ships_tinted_red"),
        GetSubspriteRect(), SDL2pp::NullOpt);
    resource_manager->AddTexture("modular_ships_hit", target2);
    renderer->SetTarget();
  }
}

void Ship::Update() {
  if (hit) {
    renderer->Copy(
        *resource_manager->GetTexture("modular_ships_hit"),
        SDL2pp::Rect{0, 0, GetSubspriteRect().w, GetSubspriteRect().h},
        position, 0, SDL2pp::NullOpt, flip);
    Uint32 now = SDL_GetTicks();
    if (now - last_hit >= 100) {
      hit = false;
    }
  } else {
    renderer->Copy(
        *resource_manager->GetTexture("modular_ships_shadowed"),
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
    bullets.push_back(
        std::make_shared<Bullet>(renderer, resource_manager, position));
  }
}

void Ship::OnHitByBullet(std::shared_ptr<Bullet> bullet) {
  hit = true;
  last_hit = SDL_GetTicks();
  stats->health -= bullet->stats->damage;
  console->info("hit for {}, {}/{}", bullet->stats->damage, stats->health,
                stats->health_max);
}
