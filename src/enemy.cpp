/* Copyright 2016 Tony Narlock. All rights reserved. */
#include "config.h"
#include "resource.h"
#include "enemy.h"
#include "util.h"

Enemy::Enemy(const std::unique_ptr<SDL2pp::Renderer>& renderer,
             const std::unique_ptr<ResourceManager>& resource_manager,
             const std::shared_ptr<spdlog::logger>& console,
             SDL2pp::Optional<SDL2pp::Point> position,
             SDL2pp::Point velocity,
             int flip)
    : ship(std::make_unique<Ship>(
          renderer,
          resource_manager,
          console,
          resource_manager->GetTexture("modular_ships_tinted_tan"),
          position,
          velocity,
          ShipStats{2, 2, 1},
          flip)) {
  ship->subsprites[static_cast<int>(Actor::ActorState::DEFAULT)] =
      ship->subsprite_rect;
  console->info("spawned new enemy at {}, {}", ship->position.x,
                ship->position.y);
}

void Enemy::Update() {
  ship->position.x--;
  ship->Update();
}
