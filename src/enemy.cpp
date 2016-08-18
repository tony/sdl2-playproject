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
    : ship(std::make_shared<Ship>(
          renderer,
          resource_manager,
          console,
          "ship1_tinted",
          position,
          velocity,
          std::make_shared<ShipStats>(ShipStats{2, 2, 1}),
          flip)) {
  console->info("spawned new enemy at {}, {}", ship->position.x,
                ship->position.y);
}

void Enemy::Update() {
  ship->position.x--;
  ship->Update();
}
