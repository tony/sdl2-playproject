/* Copyright 2016 Tony Narlock. All rights reserved. */
#include "config.h"
#include "resource.h"
#include "ship.h"
#include "util.h"

Player::Player(const std::unique_ptr<SDL2pp::Renderer>& renderer,
               const std::unique_ptr<ResourceManager>& resource_manager,
               const std::shared_ptr<spdlog::logger>& console)
    : ship(std::make_shared<Ship>(renderer,
                                  resource_manager,
                                  console,
                                  "ship1",
                                  SDL2pp::Point{30, 30},
                                  SDL2pp::Point{0, 0})) {}

void Player::HandleInput(const std::shared_ptr<InputManager>& input) {
  ship->HandleInput(input);
}
GraphicsComponent::GraphicsComponent(
    const std::unique_ptr<ResourceManager>& resource_manager)
    : resource_manager(resource_manager){};
ShipGraphicsComponent::ShipGraphicsComponent(
    const std::unique_ptr<ResourceManager>& resource_manager)
    : GraphicsComponent(resource_manager) {}

void ShipGraphicsComponent::Update(
    const std::shared_ptr<Ship>& actor,
    const std::unique_ptr<SDL2pp::Renderer>& renderer) {
  if (actor->GetHit()) {
    renderer->Copy(*resource_manager->GetTexture("ship1_hit"),
                   SDL2pp::Rect{0, 0, actor->GetSubspriteRect().w,
                                actor->GetSubspriteRect().h},
                   actor->GetPosition(), 0, SDL2pp::NullOpt, actor->GetFlip());
    Uint32 now = SDL_GetTicks();
    if (now - actor->GetLastHit() >= 100) {
      actor->SetHit(false);
    }
  } else {
    renderer->Copy(*resource_manager->GetTexture(actor->GetTextureKey()),
                   SDL2pp::Rect{0, 0, actor->GetSubspriteRect().w,
                                actor->GetSubspriteRect().h},
                   actor->GetPosition(), 0, SDL2pp::NullOpt, actor->GetFlip());
  }

  // bullet drawing and clean up
  actor->bullets.erase(
      std::remove_if(actor->bullets.begin(), actor->bullets.end(),
                     [](auto& b) { return !b->InBounds(); }),
      actor->bullets.end());
  for (auto& bullet : actor->bullets) {
    bullet->Update();
  }
}

void Ship::HandleInput(const std::shared_ptr<InputManager>& input) {
  if (input->down(SDL_SCANCODE_UP) | input->down(SDL_SCANCODE_W) |
      input->down(SDL_SCANCODE_K)) {
    state = ActorState::UP;
    position.y =
        clamp(position.y - static_cast<int>(MAIN_VIEWPORT_RECT.h * 0.01), 0,
              MAIN_VIEWPORT_RECT.h - GetSprite()->GetHeight());
  }

  if (input->down(SDL_SCANCODE_DOWN) | input->down(SDL_SCANCODE_S) |
      input->down(SDL_SCANCODE_J)) {
    state = ActorState::DOWN;
    position.y =
        clamp(position.y + static_cast<int>(MAIN_VIEWPORT_RECT.h * 0.01), 0,
              MAIN_VIEWPORT_RECT.h - GetSprite()->GetHeight());
  }

  if (input->down(SDL_SCANCODE_LEFT) | input->down(SDL_SCANCODE_A) |
      input->down(SDL_SCANCODE_H)) {
    state = ActorState::LEFT;
    position.x = clamp(position.x - static_cast<int>(SCREEN_RECT.w * 0.01), 0,
                       SCREEN_RECT.w - GetSprite()->GetWidth());
  }

  if (input->down(SDL_SCANCODE_RIGHT) | input->down(SDL_SCANCODE_D) |
      input->down(SDL_SCANCODE_L)) {
    state = ActorState::RIGHT;
    position.x = clamp(position.x + static_cast<int>(SCREEN_RECT.w * 0.01), 0,
                       SCREEN_RECT.w - GetSprite()->GetWidth());
  }

  if (input->down(SDL_SCANCODE_SPACE)) {
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
           const std::string& texture_key,
           SDL2pp::Optional<SDL2pp::Point> position,
           SDL2pp::Point velocity,
           const std::shared_ptr<ShipStats>& stats,
           int flip)
    : Actor(renderer, resource_manager, texture_key, velocity, position, flip),
      graphics_(std::make_shared<ShipGraphicsComponent>(resource_manager)),
      stats(stats),
      console(console) {}

void Ship::Update() {
  graphics_->Update(shared_from_this(), renderer);
}

void Ship::SpawnBullet() {
  if (bullets.size() < SHIP_MAX_BULLETS) {
    bullets.push_back(std::make_shared<Bullet>(
        renderer, resource_manager, "bullet1",
        SDL2pp::Point{position.x + 30, position.y + (GetSize().y / 2)}));
  }
}

void Ship::OnHitByBullet(std::shared_ptr<Bullet> bullet) {
  hit = true;
  last_hit = SDL_GetTicks();
  stats->health -= bullet->stats->damage;
  console->info("hit for {}, {}/{}", bullet->stats->damage, stats->health,
                stats->health_max);
}
